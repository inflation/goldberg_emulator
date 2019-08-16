#include "DX9_Hook.h"

#ifndef NO_OVERLAY

#include "Hook_Manager.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>
#include <impls/imgui_impl_dx9.h>

#include "steam_overlay.h"

static DX9_Hook* hook;

//////////////////////////////////////////////////////////////////
/////////                                                /////////
/////////  This hook doesn't support game resize for now /////////
/////////                                                /////////
//////////////////////////////////////////////////////////////////

void DX9_Hook::start_hook()
{
    if (!_hooked)
    {
        Hook_Manager::Inst().FoundRenderer(this);

        IDirect3D9Ex* pD3D;
        IDirect3DDevice9Ex* pDeviceEx;
        decltype(Direct3DCreate9Ex)* Direct3DCreate9Ex = (decltype(Direct3DCreate9Ex))GetProcAddress(_dll, "Direct3DCreate9Ex");

        Direct3DCreate9Ex(D3D_SDK_VERSION, &pD3D);

        D3DPRESENT_PARAMETERS params = {};
        params.BackBufferWidth = 1;
        params.BackBufferHeight = 1;
        params.hDeviceWindow = GetForegroundWindow();

        pD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, NULL, &pDeviceEx);

        if (pDeviceEx != nullptr)
        {
            _hooked = true;
            PRINT_DEBUG("Hooked DirectX 9\n");
            loadFunctions(pDeviceEx);

            UnhookAll();
            BeginHook();
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)Reset, &DX9_Hook::MyReset),
                std::make_pair<void**, void*>(&(PVOID&)Present, &DX9_Hook::MyPresent),
                std::make_pair<void**, void*>(&(PVOID&)PresentEx, &DX9_Hook::MyPresentEx)
                //std::make_pair<void**, void*>(&(PVOID&)EndScene, &DX9_Hook::MyEndScene)
            );
            EndHook();
        }
        else
        {
            PRINT_DEBUG("Failed to DirectX 9\n");
        }

        if(pDeviceEx)pDeviceEx->Release();
        if(pD3D)pD3D->Release();
    }
}

void DX9_Hook::resetRenderState()
{
    if (initialized)
    {
        initialized = false;
        ImGui_ImplDX9_Shutdown();
        ImGui_ImplWin32_Shutdown();
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
    if (param.hFocusWindow != Hook_Manager::Inst().GetOverlay()->GetGameHwnd())
        resetRenderState();

    if (!initialized)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        Hook_Manager::Inst().ChangeGameWindow(param.hFocusWindow);
        ImGui_ImplWin32_Init(param.hFocusWindow);
        ImGui_ImplDX9_Init(pDevice);
        initialized = true;
    }
    
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    Hook_Manager::Inst().CallOverlayProc(PresentParameters.BackBufferWidth, PresentParameters.BackBufferHeight);

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
    hook->resetRenderState();
    return (_this->*hook->Reset)(pPresentationParameters);
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyEndScene(IDirect3DDevice9* _this)
{   
    if( !hook->uses_present )
        hook->prepareForOverlay(_this);
    return (_this->*hook->EndScene)();
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    hook->uses_present = true;
    hook->prepareForOverlay(_this);
    return (_this->*hook->Present)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT STDMETHODCALLTYPE DX9_Hook::MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    hook->uses_present = true;
    hook->prepareForOverlay(_this);
    return (_this->*hook->PresentEx)(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

DX9_Hook::DX9_Hook():
    initialized(false),
    uses_present(false),
    EndScene(nullptr),
    Present(nullptr),
    PresentEx(nullptr),
    Reset(nullptr)
{
    _dll = GetModuleHandle(DLL_NAME);
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

    if (_hooked)
    {
        //ImGui_ImplDX9_Shutdown(); This makes some games hang when Releasing the D3D9 device (pDevice->Release())
        // maybe because D3D is already shut down when we try to free the device?
        ImGui_ImplDX9_InvalidateDeviceObjects();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    hook = nullptr;
}

void DX9_Hook::Create()
{
    if( hook == nullptr )
    {
        hook = new DX9_Hook;
        hook->start_hook();
        // Register the hook to the Hook Manager
        Hook_Manager::Inst().AddHook(hook);
    }
}

void DX9_Hook::loadFunctions(IDirect3DDevice9Ex* pDeviceEx)
{
    void** vTable = *reinterpret_cast<void***>(pDeviceEx);

#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDirect3DDevice9VTable::X]
    LOAD_FUNC(Reset);
    LOAD_FUNC(EndScene);
    LOAD_FUNC(Present);
    LOAD_FUNC(PresentEx);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY