#include "../dll/base.h"

#ifndef NO_OVERLAY

#include "DX10_Hook.h"
#include "Hook_Manager.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>
#include <impls/imgui_impl_dx10.h>

// This is created by DX10_Hook::Create, and deleted by the Hook_Manager if not used
static DX10_Hook* hook;

void DX10_Hook::start_hook()
{
    if (!_hooked)
    {
        Hook_Manager::Inst().FoundRenderer(this);

        IDXGISwapChain* pSwapChain;
        ID3D10Device* pDevice;
        DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
        decltype(D3D10CreateDeviceAndSwapChain)* D3D10CreateDeviceAndSwapChain =
            (decltype(D3D10CreateDeviceAndSwapChain))GetProcAddress(_dll, "D3D10CreateDeviceAndSwapChain");
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

        D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_NULL, NULL, 0, D3D10_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice);

        if (pDevice != nullptr && pSwapChain != nullptr)
        {
            _hooked = true;
            PRINT_DEBUG("Hooked DirectX 10\n");
            loadFunctions(pDevice, pSwapChain);

            UnhookAll();
            BeginHook();
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::Present, &DX10_Hook::MyPresent),
                std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::ResizeTarget, &DX10_Hook::MyResizeTarget),
                std::make_pair<void**, void*>(&(PVOID&)DX10_Hook::ResizeBuffers, &DX10_Hook::MyResizeBuffers)
            );
            EndHook();
        }
        else
        {
            PRINT_DEBUG("Failed to hook DirectX 10\n");
        }
        if(pDevice)pDevice->Release();
        if(pSwapChain)pSwapChain->Release();
    }
}

void DX10_Hook::resetRenderState()
{
    if (initialized)
    {
        mainRenderTargetView->Release();

        ImGui_ImplDX10_Shutdown();
        ImGui_ImplWin32_Shutdown();
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
        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D10Device), (PVOID*)& pDevice)))
            return;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ID3D10Texture2D* pBackBuffer;

        pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)& pBackBuffer);
        pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
        pBackBuffer->Release();

        Hook_Manager::Inst().ChangeGameWindow(desc.OutputWindow);
        ImGui_ImplWin32_Init(desc.OutputWindow);
        ImGui_ImplDX10_Init(pDevice);
        initialized = true;
    }

    ImGui_ImplDX10_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    Hook_Manager::Inst().CallOverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

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
    hook->prepareForOverlay(_this);
    return (_this->*hook->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX10_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX10_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX10_Hook::DX10_Hook():
    initialized(false),
    pDevice(nullptr),
    mainRenderTargetView(nullptr)
{
    _dll = GetModuleHandle(DLL_NAME);

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

    if (_hooked)
        resetRenderState();

    hook = nullptr;
}

void DX10_Hook::Create()
{
    if (hook == nullptr)
    {
        hook = new DX10_Hook;
        hook->start_hook();
        // Register the hook to the Hook Manager
        Hook_Manager::Inst().AddHook(hook);
    }
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