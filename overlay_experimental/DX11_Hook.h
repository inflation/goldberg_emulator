#ifndef __INCLUDED_DX11_HOOK_H__
#define __INCLUDED_DX11_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

#include <d3d11.h>
#include "DirectX_VTables.h"

class DX11_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "d3d11.dll";

private:
    // Variables
    bool initialized;
    ID3D11DeviceContext* pContext;
    ID3D11RenderTargetView* mainRenderTargetView;   

    // Functions
    DX11_Hook();
    virtual ~DX11_Hook();

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
    //static decltype(D3D11CreateDevice) MyD3D11CreateDevice;
    //static decltype(D3D11CreateDeviceAndSwapChain) MyD3D11CreateDeviceAndSwapChain;

    //decltype(D3D11CreateDevice)* D3D11CreateDevice;
    //decltype(D3D11CreateDeviceAndSwapChain)* D3D11CreateDeviceAndSwapChain;

public:
    bool start_hook();
    static DX11_Hook* Inst();
    
    void loadFunctions(ID3D11Device *pDevice, IDXGISwapChain *pSwapChain);
};

#endif//NO_OVERLAY

#endif//__INCLUDED_DX11_HOOK_H__