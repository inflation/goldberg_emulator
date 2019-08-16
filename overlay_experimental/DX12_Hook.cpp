#include "../dll/base.h"

#ifndef NO_OVERLAY

#include "DX12_Hook.h"
#include "Hook_Manager.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>
#include <impls/imgui_impl_dx12.h>

// This is created by DX12_Hook::Create, and deleted by the Hook_Manager if not used
static DX12_Hook* hook;

void DX12_Hook::hook_dx12(D3D_FEATURE_LEVEL MinimumFeatureLevel)
{
    if (!_hooked)
    {
        PRINT_DEBUG("Hooked DirectX 12\n");
    }
}

void DX12_Hook::resetRenderState()
{
    if (initialized)
    {
        pCmdAlloc->Release();
        pCmdList->Release();
        pDescriptorHeap->Release();

        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
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

        HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D12Device), (PVOID*)&pDevice);
        
        d3d12_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        d3d12_desc.NumDescriptors = 1;
        d3d12_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        d3d12_desc.NodeMask = 1;
        if (pDevice->CreateDescriptorHeap(&d3d12_desc, IID_PPV_ARGS(&pDescriptorHeap)) != S_OK)
        {
            pDevice->Release();
            return;
        }

        SIZE_T rtvDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        mainRenderTargetDescriptor = rtvHandle;

        pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCmdAlloc));

        pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCmdAlloc, NULL, IID_PPV_ARGS(&pCmdList));
        
        Hook_Manager::Inst().ChangeGameWindow(desc.OutputWindow);
        ImGui_ImplWin32_Init(desc.OutputWindow);
        ImGui_ImplDX12_Init(pDevice, 1, DXGI_FORMAT_R8G8B8A8_UNORM,
            pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
            pDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
        initialized = true;
    }

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    Hook_Manager::Inst().CallOverlayProc(desc.BufferDesc.Width, desc.BufferDesc.Height);

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
HRESULT WINAPI DX12_Hook::MyD3D12CreateDevice(
        _In_opt_ IUnknown* pAdapter,
        D3D_FEATURE_LEVEL MinimumFeatureLevel,
        _In_ REFIID riid, // Expected: ID3D12Device
        _COM_Outptr_opt_ void** ppDevice)
{
    auto res = hook->D3D12CreateDevice(pAdapter, MinimumFeatureLevel, riid, ppDevice);

    if (SUCCEEDED(res))
        hook->hook_dx12(MinimumFeatureLevel);

    return res;
}
/////////////////////////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE DX12_Hook::MyPresent(IDXGISwapChain *_this, UINT SyncInterval, UINT Flags)
{
    hook->prepareForOverlay(_this);

    return (_this->*hook->Present)(SyncInterval, Flags);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeTarget)(pNewTargetParameters);
}

HRESULT STDMETHODCALLTYPE DX12_Hook::MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    hook->resetRenderState();
    return (_this->*hook->ResizeBuffers)(BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

DX12_Hook::DX12_Hook():
    initialized(false),
    pCmdAlloc(nullptr),
    pCmdList(nullptr),
    pDescriptorHeap(nullptr)
{
    _dll = GetModuleHandle(DLL_NAME);

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

    if (_hooked)
        resetRenderState();

    hook = nullptr;
}

void DX12_Hook::Create()
{
    if (hook == nullptr)
    {
        hook = new DX12_Hook;
        // Register the hook to the Hook Manager
        Hook_Manager::Inst().AddHook(hook);
    }
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