#include "DX12_Hook.h"
#include "Windows_Hook.h"
#include "Hook_Manager.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_dx12.h>

#include <dxgi1_4.h>

DX12_Hook* DX12_Hook::_inst = nullptr;

bool DX12_Hook::start_hook()
{
    bool res = true;
    if (!_hooked)
    {
        if (!Windows_Hook::Inst().start_hook())
            return false;

        HWND hWnd = GetGameWindow();
        if (!hWnd)
            return false;

        IDXGIFactory4* pDXGIFactory = nullptr;
        IDXGISwapChain1* pSwapChain = nullptr;
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        ID3D12CommandQueue* pCommandQueue = nullptr;
        ID3D12Device* pDevice = nullptr;
        decltype(D3D12CreateDevice)* D3D12CreateDevice =
            (decltype(D3D12CreateDevice))GetProcAddress(GetModuleHandle(DX12_Hook::DLL_NAME), "D3D12CreateDevice");

        D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&pDevice));

        if (pDevice)
        {
            DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
            SwapChainDesc.Width = 0;
            SwapChainDesc.Height = 0;
            SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SwapChainDesc.Stereo = FALSE;
            SwapChainDesc.SampleDesc = { 1, 0 };
            SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            SwapChainDesc.BufferCount = 3;
            SwapChainDesc.Scaling = DXGI_SCALING_STRETCH;
            SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pCommandQueue));

            if (pCommandQueue)
            {
                reinterpret_cast<decltype(CreateDXGIFactory1)*>(GetProcAddress(GetModuleHandle("dxgi.dll"), "CreateDXGIFactory1"))(IID_PPV_ARGS(&pDXGIFactory));
                pDXGIFactory->CreateSwapChainForHwnd(pCommandQueue, hWnd, &SwapChainDesc, NULL, NULL, &pSwapChain);

                if (pSwapChain != nullptr)
                {
                    PRINT_DEBUG("Hooked DirectX 12\n");

                    _hooked = true;
                    Hook_Manager::Inst().FoundRenderer(this);

                    loadFunctions(pDevice, pSwapChain);

                    UnhookAll();
                    BeginHook();
                    HookFuncs(
                        std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::Present, &DX12_Hook::MyPresent),
                        std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::ResizeTarget, &DX12_Hook::MyResizeTarget),
                        std::make_pair<void**, void*>(&(PVOID&)DX12_Hook::ResizeBuffers, &DX12_Hook::MyResizeBuffers)
                    );
                    EndHook();

                    get_steam_client()->steam_overlay->HookReady();
                }
                else
                {
                    PRINT_DEBUG("Failed to hook DirectX 12\n");
                    res = false;
                }
            }
        }

        if (pSwapChain) pSwapChain->Release();
        if (pDXGIFactory) pDXGIFactory->Release();
        if (pCommandQueue) pCommandQueue->Release();
        if (pDevice) pDevice->Release();
    }
    return res;
}

void DX12_Hook::resetRenderState()
{
    if (initialized)
    {
        pCmdAlloc->Release();
        pCmdList->Release();
        pDescriptorHeap->Release();

        ImGui_ImplDX12_Shutdown();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void DX12_Hook::prepareForOverlay(IDXGISwapChain* pSwapChain)
{
    DXGI_SWAP_CHAIN_DESC desc;
    pSwapChain->GetDesc(&desc);

    if (!initialized)
    {
        D3D12_DESCRIPTOR_HEAP_DESC d3d12_desc = {};
        ID3D12Device* pDevice;

        if (!SUCCEEDED(pSwapChain->GetDevice(IID_PPV_ARGS(&pDevice))))
            return;

        d3d12_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        d3d12_desc.NumDescriptors = 1;
        d3d12_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        d3d12_desc.NodeMask = 1;
        if (pDevice->CreateDescriptorHeap(&d3d12_desc, IID_PPV_ARGS(&pDescriptorHeap)) != S_OK)
        {
            pDevice->Release();
            return;
        }

        mainRenderTargetDescriptor = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        if (!SUCCEEDED(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAlloc))))
        {
            pDescriptorHeap->Release();
            pDevice->Release();
            return;
        }
        if (!SUCCEEDED(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAlloc, NULL, IID_PPV_ARGS(&pCmdList))))
        {
            pCmdAlloc->Release();
            pDescriptorHeap->Release();
            pDevice->Release();
            return;
        }

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ImGui_ImplDX12_Init(pDevice, 1, DXGI_FORMAT_R8G8B8A8_UNORM,
            pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    
        pCmdList->Release();
        pCmdAlloc->Release();
        pDescriptorHeap->Release();
        pDevice->Release();
        initialized = true;
    }

    ImGui_ImplDX12_NewFrame();
    Windows_Hook::Inst().prepareForOverlay(desc.OutputWindow);

    ImGui::NewFrame();

    get_steam_client()->steam_overlay->OverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

    ImGui::EndFrame();

    ImGui::Render();

    pCmdAlloc->Reset();
    pCmdList->Reset(pCmdAlloc, NULL);
    //pCmdList->ClearRenderTargetView(mainRenderTargetDescriptor, (float*)& clear_color, 0, NULL);
    pCmdList->OMSetRenderTargets(1, &mainRenderTargetDescriptor, FALSE, NULL);
    pCmdList->SetDescriptorHeaps(1, &pDescriptorHeap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), pCmdList);
}

/////////////////////////////////////////////////////////////////////////////////////
// DirectX 12 Initialization functions
//HRESULT WINAPI DX12_Hook::MyD3D12CreateDevice(
//        _In_opt_ IUnknown* pAdapter,
//        D3D_FEATURE_LEVEL MinimumFeatureLevel,
//        _In_ REFIID riid, // Expected: ID3D12Device
//        _COM_Outptr_opt_ void** ppDevice)
//{
//    auto res = hook->D3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);
//
//    if (SUCCEEDED(res))
//        hook->hook_dx12(MinimumFeatureLevel);
//
//    return res;
//}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX12_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    DX12_Hook::Inst()->prepareForOverlay(_this);

    return (_this->*DX12_Hook::Inst()->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    DX12_Hook::Inst()->resetRenderState();
    return (_this->*DX12_Hook::Inst()->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    DX12_Hook::Inst()->resetRenderState();
    return (_this->*DX12_Hook::Inst()->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX12_Hook::DX12_Hook():
    initialized(false),
    pCmdAlloc(nullptr),
    pCmdList(nullptr),
    pDescriptorHeap(nullptr)
{
    _library = LoadLibrary(DLL_NAME);

    PRINT_DEBUG("Trying to hook DX12 but DX12_Hook is not implemented yet, please report to DEV with the game name.");

    // Hook to D3D12CreateDevice and D3D12CreateDeviceAndSwapChain so we know when it gets called.
    // If its called, then DX12 will be used to render the overlay.
    //D3D12CreateDevice = (decltype(D3D12CreateDevice))GetProcAddress(_dll, "D3D12CreateDevice");
    //
    //BeginHook();
    //HookFuncs(
    //    std::make_pair<void**, void*>(&(PVOID&)D3D12CreateDevice, &DX12_Hook::MyD3D12CreateDevice)
    //);
    //EndHook();
}

DX12_Hook::~DX12_Hook()
{
    PRINT_DEBUG("DX12 Hook removed\n");

    if (initialized)
    {
        pCmdAlloc->Release();
        pCmdList->Release();
        pDescriptorHeap->Release();

        //ImGui_ImplDX12_Shutdown();
        ImGui_ImplDX12_InvalidateDeviceObjects();
        Windows_Hook::Inst().resetRenderState();
        ImGui::DestroyContext();

        initialized = false;
    }

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

DX12_Hook* DX12_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new DX12_Hook();

    return _inst;
}

void DX12_Hook::loadFunctions(ID3D12Device *pDevice, IDXGISwapChain *pSwapChain)
{
    void** vTable = *reinterpret_cast<void***>(pDevice);

    /*
#define LOAD_FUNC(X) (void*&)X = vTable[(int)ID3D12DeviceVTable::X]

#undef LOAD_FUNC
    */
    vTable = *reinterpret_cast<void***>(pSwapChain);
#define LOAD_FUNC(X) (void*&)X = vTable[(int)IDXGISwapChainVTable::X]
    LOAD_FUNC(Present);
    LOAD_FUNC(ResizeBuffers);
    LOAD_FUNC(ResizeTarget);
#undef LOAD_FUNC
}

#endif//NO_OVERLAY