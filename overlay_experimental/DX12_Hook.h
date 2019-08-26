#ifndef __INCLUDED_DX12_HOOK_H__
#define __INCLUDED_DX12_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

#include <d3d12.h>
#include <dxgi1_4.h>
#include "DirectX_VTables.h"

class DX12_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "d3d12.dll";

private:
    static DX12_Hook* _inst;

    // Variables
    bool initialized;

    DXGI_SWAP_CHAIN_DESC sc_desc;
    ID3D12DescriptorHeap* pSrvDescHeap;

    // Functions
    DX12_Hook();
    virtual ~DX12_Hook();

    void resetRenderState();
    void prepareForOverlay(IDXGISwapChain* pSwapChain);

    // Hook to render functions
    static HRESULT STDMETHODCALLTYPE MyPresent(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags);
    static HRESULT STDMETHODCALLTYPE MyResizeTarget(IDXGISwapChain* _this, const DXGI_MODE_DESC* pNewTargetParameters);
    static HRESULT STDMETHODCALLTYPE MyResizeBuffers(IDXGISwapChain* _this, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    static void STDMETHODCALLTYPE MyExecuteCommandLists(ID3D12CommandQueue *_this, UINT NumCommandLists, ID3D12CommandList* const* ppCommandLists);
    static HRESULT STDMETHODCALLTYPE MyClose(ID3D12GraphicsCommandList* _this);

    decltype(&IDXGISwapChain::Present)       Present;
    decltype(&IDXGISwapChain::ResizeBuffers) ResizeBuffers;
    decltype(&IDXGISwapChain::ResizeTarget)  ResizeTarget;
    decltype(&ID3D12CommandQueue::ExecuteCommandLists) ExecuteCommandLists;
    decltype(&ID3D12GraphicsCommandList::Close) Close;

    // Hook functions so we know we use DX11
    //static decltype(D3D12CreateDevice) MyD3D12CreateDevice;

    //decltype(D3D12CreateDevice)* D3D12CreateDevice;

public:
    bool start_hook();
    static DX12_Hook* Inst();
    virtual const char* get_lib_name() const;
    
    void loadFunctions(ID3D12Device *pDevice, ID3D12CommandQueue *pCommandQueue, IDXGISwapChain *pSwapChain);
};

#endif//NO_OVERLAY
#endif//__INCLUDED_DX12_HOOK_H__