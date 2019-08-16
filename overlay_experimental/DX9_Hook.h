#ifndef __INCLUDED_DX9_HOOK_H__
#define __INCLUDED_DX9_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

#include <d3d9.h>
#include "DirectX_VTables.h"

class DX9_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "d3d9.dll";

private:
    // Variables
    bool initialized;
    bool uses_present;

    // Functions
    DX9_Hook();
    virtual ~DX9_Hook();

    void resetRenderState();
    void prepareForOverlay(IDirect3DDevice9* pDevice);

    // Hook to render functions
    decltype(&IDirect3DDevice9::Reset)       Reset;
    decltype(&IDirect3DDevice9::EndScene)    EndScene;
    decltype(&IDirect3DDevice9::Present)     Present;
    decltype(&IDirect3DDevice9Ex::PresentEx) PresentEx;

    static HRESULT STDMETHODCALLTYPE MyReset(IDirect3DDevice9* _this, D3DPRESENT_PARAMETERS* pPresentationParameters);
    static HRESULT STDMETHODCALLTYPE MyEndScene(IDirect3DDevice9 *_this);
    static HRESULT STDMETHODCALLTYPE MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    static HRESULT STDMETHODCALLTYPE MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);

    // Hook functions so we know we use DX9
    //static decltype(Direct3DCreate9) MyDirect3DCreate9;
    //static decltype(Direct3DCreate9Ex) MyDirect3DCreate9Ex;

    //decltype(Direct3DCreate9)* Direct3DCreate9;
    //decltype(Direct3DCreate9Ex)* Direct3DCreate9Ex;

public:
    bool start_hook();
    static DX9_Hook* Inst();
    
    void loadFunctions(IDirect3DDevice9Ex *pDeviceEx);
};

#endif//NO_OVERLAY

#endif//__INCLUDED_DX9_HOOK_H__