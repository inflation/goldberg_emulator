#include "DX11_Hook.h"
#include "Windows_Hook.h"
#include "Renderer_Detector.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_dx11.h>

DX11_Hook* DX11_Hook::_inst = nullptr;

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
    HRESULT ret = pSwapChain->GetDevice(IID_PPV_ARGS(ppDevice));

    if (SUCCEEDED(ret))
        (*ppDevice)->GetImmediateContext(ppContext);

    return ret;
}

bool DX11_Hook::start_hook()
{
    bool res = true;
    if (!hooked)
    {
        if (!Windows_Hook::Inst()->start_hook())
            return false;

        PRINT_DEBUG("Hooked DirectX 11\n");
        hooked = true;

        Renderer_Detector::Inst().renderer_found(this);

        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::Present, &DX11_Hook::MyPresent),
            std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::ResizeTarget, &DX11_Hook::MyResizeTarget),
            std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::ResizeBuffers, &DX11_Hook::MyResizeBuffers)
        );
        EndHook();

        get_steam_client()->steam_overlay->HookReady();
    }
    return res;
}

void DX11_Hook::resetRenderState()
{
    if (initialized)
    {
        mainRenderTargetView->Release();
        pContext->Release();

        ImGui_ImplDX11_Shutdown();
        Windows_Hook::Inst()->resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void DX11_Hook::prepareForOverlay(IDXGISwapChain* pSwapChain)
{
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    if (!initialized)
    {
        ID3D11Device* pDevice = nullptr;
        if (FAILED(GetDeviceAndCtxFromSwapchain(pSwapChain, &pDevice, &pContext)))
            return;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ID3D11Texture2D* pBackBuffer;

        pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
        pBackBuffer->Release();

        ImGui_ImplDX11_Init(pDevice, pContext);

        pDevice->Release();

        initialized = true;
    }

    ImGui_ImplDX11_NewFrame();
    Windows_Hook::Inst()->prepareForOverlay(desc.OutputWindow);

    ImGui::NewFrame();

    get_steam_client()->steam_overlay->OverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

    ImGui::EndFrame();

    ImGui::Render();

    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

/////////////////////////////////////////////////////////////////////////////////////
// DirectX 11 Initialization functions
//HRESULT WINAPI DX11_Hook::MyD3D11CreateDevice(__in_opt IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, 
//    __in_ecount_opt(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, __out_opt ID3D11Device** ppDevice,
//    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel, __out_opt ID3D11DeviceContext** ppImmediateContext)
//{
//    auto res = D3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
//
//    if (SUCCEEDED(res))
//        hook->hook_dx11(SDKVersion);
//
//    return res;
//}

//HRESULT WINAPI DX11_Hook::MyD3D11CreateDeviceAndSwapChain(__in_opt IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
//    __in_ecount_opt(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, 
//    __in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, __out_opt IDXGISwapChain** ppSwapChain, __out_opt ID3D11Device** ppDevice,
//    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel, __out_opt ID3D11DeviceContext** ppImmediateContext)
//{
//    auto res = hook->D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);    
//
//    if (SUCCEEDED(res))
//        hook->hook_dx11(SDKVersion);
//
//    return res;
//}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX11_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    DX11_Hook::Inst()->prepareForOverlay(_this);

    return (_this->*DX11_Hook::Inst()->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX11_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    DX11_Hook::Inst()->resetRenderState();
    return (_this->*DX11_Hook::Inst()->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX11_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    DX11_Hook::Inst()->resetRenderState();
    return (_this->*DX11_Hook::Inst()->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX11_Hook::DX11_Hook():
    initialized(false),
    hooked(false),
    pContext(nullptr),
    mainRenderTargetView(nullptr),
    Present(nullptr),
    ResizeBuffers(nullptr),
    ResizeTarget(nullptr)
{
    _library = LoadLibrary(DLL_NAME);
    
    // Hook to D3D11CreateDevice and D3D11CreateDeviceAndSwapChain so we know when it gets called.
    // If its called, then DX11 will be used to render the overlay.
    //D3D11CreateDevice = (decltype(D3D11CreateDevice))GetProcAddress(_dll, "D3D11CreateDevice");
    //D3D11CreateDeviceAndSwapChain = (decltype(D3D11CreateDeviceAndSwapChain))GetProcAddress(_dll, "D3D11CreateDeviceAndSwapChain");
    //
    //BeginHook();
    //HookFuncs(
    //    //std::make_pair<void**, void*>(&(PVOID&)D3D11CreateDevice, &DX11_Hook::MyD3D11CreateDevice),
    //    std::make_pair<void**, void*>(&(PVOID&)D3D11CreateDeviceAndSwapChain, &DX11_Hook::MyD3D11CreateDeviceAndSwapChain)
    //);
    //EndHook();
}

DX11_Hook::~DX11_Hook()
{
    PRINT_DEBUG("DX11 Hook removed\n");

    if (initialized)
    {
        mainRenderTargetView->Release();
        pContext->Release();

        //ImGui_ImplDX11_Shutdown();
        ImGui_ImplDX11_InvalidateDeviceObjects();
        ImGui::DestroyContext();

        initialized = false;
    }

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

DX11_Hook* DX11_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new DX11_Hook;

    return _inst;
}

const char* DX11_Hook::get_lib_name() const
{
    return DLL_NAME;
}

void DX11_Hook::loadFunctions(IDXGISwapChain *pSwapChain)
{
    void** vTable;

    vTable = *reinterpret_cast<void***>(pSwapChain);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDXGISwapChainVTable::X]
    LOAD_FUNC(Present);
    LOAD_FUNC(ResizeBuffers);
    LOAD_FUNC(ResizeTarget);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY