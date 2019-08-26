#include "DX10_Hook.h"
#include "Windows_Hook.h"
#include "Hook_Manager.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_dx10.h>

DX10_Hook* DX10_Hook::_inst = nullptr;

bool DX10_Hook::start_hook()
{
    bool res = true;
    if (!_hooked)
    {
        if (!Windows_Hook::Inst().start_hook())
            return false;

        PRINT_DEBUG("Hooked DirectX 10\n");
        _hooked = true;

        Hook_Manager::Inst().FoundRenderer(this);

        UnhookAll();
        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::Present, &DX10_Hook::MyPresent),
            std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::ResizeTarget, &DX10_Hook::MyResizeTarget),
            std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::ResizeBuffers, &DX10_Hook::MyResizeBuffers)
        );
        EndHook();

        get_steam_client()->steam_overlay->HookReady();
    }
    return res;
}

void DX10_Hook::resetRenderState()
{
    if (initialized)
    {
        mainRenderTargetView->Release();

        ImGui_ImplDX10_Shutdown();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void DX10_Hook::prepareForOverlay(IDXGISwapChain* pSwapChain)
{
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    if (!initialized)
    {
        if (FAILED(pSwapChain->GetDevice(IID_PPV_ARGS(&pDevice))))
            return;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ID3D10Texture2D* pBackBuffer;

        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
        pBackBuffer->Release();

        ImGui_ImplDX10_Init(pDevice);

        pDevice->Release();

        initialized = true;
    }

    ImGui_ImplDX10_NewFrame();
    Windows_Hook::Inst().prepareForOverlay(desc.OutputWindow);

    ImGui::NewFrame();

    get_steam_client()->steam_overlay->OverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

    ImGui::EndFrame();

    ImGui::Render();

    pDevice->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
}

/////////////////////////////////////////////////////////////////////////////////////
// DirectX 10 Initialization functions
//HRESULT WINAPI DX10_Hook::MyD3D10CreateDevice(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion, ID3D10Device** ppDevice)
//{
//    auto res = _D3D10CreateDevice(pAdapter, DriverType, Software, Flags, SDKVersion, ppDevice);
//
//    if (SUCCEEDED(res))
//        hook->hook_dx10(SDKVersion);
//
//    return res;
//}

//HRESULT WINAPI DX10_Hook::MyD3D10CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D10_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, UINT SDKVersion,
//    DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D10Device** ppDevice)
//{
//    auto res = hook->D3D10CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice);
//
//    if (SUCCEEDED(res))
//        hook->hook_dx10(SDKVersion);
//
//    return res;
//}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX10_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    DX10_Hook::Inst()->prepareForOverlay(_this);
    return (_this->*DX10_Hook::Inst()->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX10_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    DX10_Hook::Inst()->resetRenderState();
    return (_this->*DX10_Hook::Inst()->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX10_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    DX10_Hook::Inst()->resetRenderState();
    return (_this->*DX10_Hook::Inst()->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX10_Hook::DX10_Hook():
    initialized(false),
    pDevice(nullptr),
    mainRenderTargetView(nullptr)
{
    _library = LoadLibrary(DLL_NAME);

    // Hook to D3D10CreateDevice and D3D10CreateDeviceAndSwapChain so we know when it gets called.
    // If its called, then DX10 will be used to render the overlay.
    //_D3D10CreateDevice = (decltype(_D3D10CreateDevice))GetProcAddress(_dll, "D3D10CreateDevice");
    //D3D10CreateDeviceAndSwapChain = (decltype(D3D10CreateDeviceAndSwapChain))GetProcAddress(_dll, "D3D10CreateDeviceAndSwapChain");
    //
    //BeginHook();
    //HookFuncs(
    //    //std::make_pair<void**, void*>(&(PVOID&)_D3D10CreateDevice, &DX10_Hook::MyD3D10CreateDevice),
    //    std::make_pair<void**, void*>(&(PVOID&)D3D10CreateDeviceAndSwapChain, &DX10_Hook::MyD3D10CreateDeviceAndSwapChain)
    //);
    //EndHook();
}

DX10_Hook::~DX10_Hook()
{
    PRINT_DEBUG("DX10 Hook removed\n");

    if (initialized)
    {
        mainRenderTargetView->Release();

        //ImGui_ImplDX10_Shutdown();
        ImGui_ImplDX10_InvalidateDeviceObjects();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

DX10_Hook* DX10_Hook::Inst()
{
    if (_inst == nullptr)   
        _inst = new DX10_Hook;

    return _inst;
}

const char* DX10_Hook::get_lib_name() const
{
    return DLL_NAME;
}

void DX10_Hook::loadFunctions(ID3D10Device *pDevice, IDXGISwapChain *pSwapChain)
{
    void** vTable = *reinterpret_cast<void***>(pDevice);

#define LOAD_FUNC(X) (void*&)X = vTable[(int)ID3D10DeviceVTable::X]
    
#undef LOAD_FUNC

    vTable = *reinterpret_cast<void***>(pSwapChain);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDXGISwapChainVTable::X]
    LOAD_FUNC(Present);
    LOAD_FUNC(ResizeBuffers);
    LOAD_FUNC(ResizeTarget);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY