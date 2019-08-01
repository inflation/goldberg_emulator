#ifndef __INCLUDED_DX12_HOOK_H__
#define __INCLUDED_DX12_HOOK_H__

#include <d3d12.h>
#include "DirectX_VTables.h"
#include "Base_Hook.h"

class DX12_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "d3d12.dll";

private:
    // Variables
    bool initialized;
    ID3D12CommandAllocator* pCmdAlloc;
    ID3D12GraphicsCommandList* pCmdList;
    ID3D12DescriptorHeap* pDescriptorHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE mainRenderTargetDescriptor;

    // Functions
    DX12_Hook();
    virtual ~DX12_Hook();

    void hook_dx12(D3D_FEATURE_LEVEL MinimumFeatureLevel);
    void resetRenderState();
    void prepareForOverlay(IDXGISwapChain* pSwapChain);

    // Hook to render functions
    static HRESULT STDMETHODCALLTYPE MyPresent(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags);
    static HRESULT STDMETHODCALLTYPE MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters);
    static HRESULT STDMETHODCALLTYPE MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

    decltype(&IDXGISwapChain::Present)       Present;
    decltype(&IDXGISwapChain::ResizeBuffers) ResizeBuffers;
    decltype(&IDXGISwapChain::ResizeTarget)  ResizeTarget;

    // Hook functions so we know we use DX11
    static decltype(D3D12CreateDevice) MyD3D12CreateDevice;

    decltype(D3D12CreateDevice)* D3D12CreateDevice;

public:
    static void Create(); // Initialize DX11 Hook.
    
    void loadFunctions(ID3D12Device *pDevice, IDXGISwapChain *pSwapChain);
};

#endif//__INCLUDED_DX12_HOOK_H__