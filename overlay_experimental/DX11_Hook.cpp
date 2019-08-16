#include "../dll/base.h"

#ifndef NO_OVERLAY

#include "DX11_Hook.h"
#include "Hook_Manager.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>
#include <impls/imgui_impl_dx11.h>

// This is created by DX11_Hook::Create, and deleted by the Hook_Manager if not used
static DX11_Hook* hook;

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain* pSwapChain, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
    HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

    if (SUCCEEDED(ret))
        (*ppDevice)->GetImmediateContext(ppContext);

    return ret;
}

void DX11_Hook::start_hook()
{
    if (!_hooked)
    {
        Hook_Manager::Inst().FoundRenderer(this);
        Hook_Manager::Inst().FoundHook(this);

        IDXGISwapChain* pSwapChain;
        ID3D11Device* pDevice;
        DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
        SwapChainDesc.BufferCount = 1;
        SwapChainDesc.BufferDesc.Width = 1;
        SwapChainDesc.BufferDesc.Height = 1;
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.OutputWindow = GetForegroundWindow();
        SwapChainDesc.SampleDesc.Count = 1;
        SwapChainDesc.SampleDesc.Quality = 0;
        SwapChainDesc.Windowed = TRUE;

        D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, NULL, D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice, NULL, NULL);

        if (pDevice != nullptr && pSwapChain != nullptr)
        {
            _hooked = true;
            PRINT_DEBUG("Hooked DirectX 11\n");
            loadFunctions(pDevice, pSwapChain);

            UnhookAll();
            BeginHook();
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::Present, &DX11_Hook::MyPresent),
                std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::ResizeTarget, &DX11_Hook::MyResizeTarget),
                std::make_pair<void**, void*>(&(PVOID&)DX11_Hook::ResizeBuffers, &DX11_Hook::MyResizeBuffers)
            );
            EndHook();
        }
        else
        {
            PRINT_DEBUG("Failed to hook DirectX 11\n");
        }

        if(pDevice) pDevice->Release();
        if(pSwapChain) pSwapChain->Release();
    }
}

void DX11_Hook::resetRenderState()
{
    if (initialized)
    {
        mainRenderTargetView->Release();
        pContext->Release();

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
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

        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
        pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
        pBackBuffer->Release();

        Hook_Manager::Inst().ChangeGameWindow(desc.OutputWindow);
        ImGui_ImplWin32_Init(desc.OutputWindow);
        ImGui_ImplDX11_Init(pDevice, pContext);
        initialized = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    Hook_Manager::Inst().CallOverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

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

HRESULT WINAPI DX11_Hook::MyD3D11CreateDeviceAndSwapChain(__in_opt IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
    __in_ecount_opt(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, 
    __in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, __out_opt IDXGISwapChain** ppSwapChain, __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel, __out_opt ID3D11DeviceContext** ppImmediateContext)
{
    auto res = hook->D3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);    

    if (SUCCEEDED(res))
        hook->hook_dx11(SDKVersion);

    return res;
}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX11_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    hook->prepareForOverlay(_this);

    return (_this->*hook->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX11_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX11_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX11_Hook::DX11_Hook():
    initialized(false),
    pContext(nullptr),
    mainRenderTargetView(nullptr)
{
    _dll = GetModuleHandle(DLL_NAME);
    
    // Hook to D3D11CreateDevice and D3D11CreateDeviceAndSwapChain so we know when it gets called.
    // If its called, then DX11 will be used to render the overlay.
    //D3D11CreateDevice = (decltype(D3D11CreateDevice))GetProcAddress(_dll, "D3D11CreateDevice");
    D3D11CreateDeviceAndSwapChain = (decltype(D3D11CreateDeviceAndSwapChain))GetProcAddress(_dll, "D3D11CreateDeviceAndSwapChain");

    BeginHook();
    HookFuncs(
        //std::make_pair<void**, void*>(&(PVOID&)D3D11CreateDevice, &DX11_Hook::MyD3D11CreateDevice),
        std::make_pair<void**, void*>(&(PVOID&)D3D11CreateDeviceAndSwapChain, &DX11_Hook::MyD3D11CreateDeviceAndSwapChain)
    );
    EndHook();

}

DX11_Hook::~DX11_Hook()
{
    PRINT_DEBUG("DX11 Hook removed\n");

    if (_hooked)
        resetRenderState();

    hook = nullptr;
}

void DX11_Hook::Create()
{
    if (hook == nullptr)
    {
        hook = new DX11_Hook;
        // Register the hook to the Hook Manager
        Hook_Manager::Inst().AddHook(hook);
    }
}

void DX11_Hook::loadFunctions(ID3D11Device *pDevice, IDXGISwapChain *pSwapChain)
{
    void** vTable = *reinterpret_cast<void***>(pDevice);

#define LOAD_FUNC(X) (void*&)X = vTable[(int)ID3D11DeviceVTable::X]

#undef LOAD_FUNC

    vTable = *reinterpret_cast<void***>(pSwapChain);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDXGISwapChainVTable::X]
    LOAD_FUNC(Present);
    LOAD_FUNC(ResizeBuffers);
    LOAD_FUNC(ResizeTarget);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY