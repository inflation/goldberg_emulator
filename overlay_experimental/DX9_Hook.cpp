#include "DX9_Hook.h"
#include "Windows_Hook.h"
#include "Hook_Manager.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_dx9.h>

#include "steam_overlay.h"

DX9_Hook* DX9_Hook::_inst = nullptr;

bool DX9_Hook::start_hook()
{
    if (!_hooked)
    {
        if (!Windows_Hook::Inst().start_hook())
            return false;

        PRINT_DEBUG("Hooked DirectX 9\n");
        _hooked = true;

        Hook_Manager::Inst().FoundRenderer(this);
        
        UnhookAll();
        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(PVOID&)Reset, &DX9_Hook::MyReset),
            std::make_pair<void**, void*>(&(PVOID&)Present, &DX9_Hook::MyPresent)
        );
        if (PresentEx != nullptr)
        {
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)PresentEx, &DX9_Hook::MyPresentEx)
                //std::make_pair<void**, void*>(&(PVOID&)EndScene, &DX9_Hook::MyEndScene)
            );
        }
        EndHook();

        get_steam_client()->steam_overlay->HookReady();
    }
    return true;
}

void DX9_Hook::resetRenderState()
{
    if (initialized)
    {
        initialized = false;
        ImGui_ImplDX9_Shutdown();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void DX9_Hook::prepareForOverlay(IDirect3DDevice9 *pDevice)
{
    IDirect3DSwapChain9* pSwapChain;
    pDevice->GetSwapChain(0, &pSwapChain);
    D3DPRESENT_PARAMETERS PresentParameters;
    pSwapChain->GetPresentParameters(&PresentParameters);
    pSwapChain->Release();

    D3DDEVICE_CREATION_PARAMETERS param;
    pDevice->GetCreationParameters(&param);

    // Workaround to detect if we changed window.
    if (param.hFocusWindow != Windows_Hook::Inst().GetGameHwnd())
        resetRenderState();

    if (!initialized)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ImGui_ImplDX9_Init(pDevice);
        initialized = true;
    }
    
    ImGui_ImplDX9_NewFrame();
    Windows_Hook::Inst().prepareForOverlay(param.hFocusWindow);

    ImGui::NewFrame();

    get_steam_client()->steam_overlay->OverlayProc(PresentParameters.BackBufferWidth, PresentParameters.BackBufferHeight);

    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

/////////////////////////////////////////////////////////////////////////////////////
// DirectX 9 Initialization functions
//IDirect3D9* DX9_Hook::MyDirect3DCreate9(UINT SDKVersion)
//{
//    auto res = hook->Direct3DCreate9(SDKVersion);
//
//    if( res != nullptr )
//        hook->hook_dx9(SDKVersion);
//
//    return res;
//}
//
//HRESULT DX9_Hook::MyDirect3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex** ppDevice)
//{
//    auto res = hook->Direct3DCreate9Ex(SDKVersion, ppDevice);
//
//    if (SUCCEEDED(res))
//        hook->hook_dx9(SDKVersion);
//
//    return res;
//}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX9_Hook::MyReset(IDirect3DDevice9* _this, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    DX9_Hook::Inst()->resetRenderState();
    return (_this->*DX9_Hook::Inst()->Reset)(pPresentationParameters);
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyEndScene(IDirect3DDevice9* _this)
{   
    if( !DX9_Hook::Inst()->uses_present )
        DX9_Hook::Inst()->prepareForOverlay(_this);
    return (_this->*DX9_Hook::Inst()->EndScene)();
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    DX9_Hook::Inst()->uses_present = true;
    DX9_Hook::Inst()->prepareForOverlay(_this);
    return (_this->*DX9_Hook::Inst()->Present)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    DX9_Hook::Inst()->uses_present = true;
    DX9_Hook::Inst()->prepareForOverlay(_this);
    return (_this->*DX9_Hook::Inst()->PresentEx)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

DX9_Hook::DX9_Hook():
    initialized(false),
    uses_present(false),
    EndScene(nullptr),
    Present(nullptr),
    PresentEx(nullptr),
    Reset(nullptr)
{
    _library = LoadLibrary(DLL_NAME);
    // Hook to Direct3DCreate9 and Direct3DCreate9Ex so we know when it gets called.
    // If its called, then DX9 will be used to render the overlay.
    //Direct3DCreate9 = (decltype(Direct3DCreate9))GetProcAddress(_dll, "Direct3DCreate9");
    //Direct3DCreate9Ex = (decltype(Direct3DCreate9Ex))GetProcAddress(_dll, "Direct3DCreate9Ex");
    //
    //BeginHook();
    //HookFuncs(
    //    std::make_pair<void**, void*>(&(PVOID&)Direct3DCreate9, &DX9_Hook::MyDirect3DCreate9),
    //    std::make_pair<void**, void*>(&(PVOID&)Direct3DCreate9Ex, &DX9_Hook::MyDirect3DCreate9Ex)
    //);
    //EndHook();
}

DX9_Hook::~DX9_Hook()
{
    PRINT_DEBUG("DX9 Hook removed\n");

    //resetRenderState();
    if (initialized)
    {
        //ImGui_ImplDX9_Shutdown(); This makes some games hang when Releasing the D3D9 device (pDevice->Release())
        // maybe because D3D is already shut down when we try to free the device?
        ImGui_ImplDX9_InvalidateDeviceObjects();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();
    }

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

DX9_Hook* DX9_Hook::Inst()
{
    if( _inst == nullptr )
        _inst = new DX9_Hook;

    return _inst;
}

const char* DX9_Hook::get_lib_name() const
{
    return DLL_NAME;
}

void DX9_Hook::loadFunctions(IDirect3DDevice9* pDevice, bool ex)
{
    void** vTable = *reinterpret_cast<void***>(pDevice);

#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDirect3DDevice9VTable::X]
    LOAD_FUNC(Reset);
    LOAD_FUNC(EndScene);
    LOAD_FUNC(Present);
    if (ex)
        LOAD_FUNC(PresentEx);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY