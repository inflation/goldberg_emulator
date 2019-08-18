#ifndef __INCLUDED_HOOK_BASE_H__
#define __INCLUDED_HOOK_BASE_H__

#include "Base_Hook.h"

#ifndef NO_OVERLAY

#include <vector>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>

struct IDXGISwapChain;
struct IDirect3DDevice9;
struct IDirect3DDevice9Ex;
#endif

/* 
 *
 */
class Hook_Manager
{
    friend class Base_Hook;
public:
    using overlayProc_t = void(*)(int,int);
    using hookReady_t = void(*)(void*);

protected:
    // TODO: If needed, create a second vector with only the renderers hook
    // Cause actually, a call to FoundRenderer will unhook everything registered except the renderer hook
    // If you do that, you should consider moving the renderer hooks to its own class and keep this one generic ?
    std::vector<Base_Hook*> _hooks; 

    unsigned int _hook_retries;
    bool _renderer_found;       // Is the renderer hooked ?
    bool _ogl_hooked;           // wglMakeCurrent is hooked ? (opengl)
    Base_Hook* rendererdetect_hook;
    class Steam_Overlay* overlay;

    Hook_Manager();
    virtual ~Hook_Manager();
    
    // Setup opengl device
    void hook_opengl();

    bool stop_retry();
    void HookLoadLibrary();

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    bool _loadlibrary_hooked;   // Are the LoadLibrary functions hooked ?
    bool _dx9_hooked;           // DX9 Present and PresentEx Hooked ?
    bool _dxgi_hooked;          // DXGI Present is hooked ? (DX10, DX11, DX12)

    // DXGIPresent will be used to detect if DX10, DX11 or DX12 should be used for overlay
    void HookDXGIPresent();
    // DX9 Present and PresentEx will be used to detect if DX9 should be used for overlay
    void HookDX9Present();
    // wglMakeCurrent will be used to detect if OpenGL3 should be used for overlay
    void HookwglMakeCurrent();
    // Setup DX9 Device and get vtable
    void hook_dx9();
    // Setup DX10 Device and get vtable
    void hook_dx10();
    // Setup DX11 Device and get vtable
    void hook_dx11();
    // Setup DX12 Device and get vtable
    void hook_dx12();

    void create_hookA(const char* libname);
    void create_hookW(const wchar_t* libname);

    static HMODULE WINAPI MyLoadLibraryA(LPCTSTR lpLibFileName);
    static HMODULE WINAPI MyLoadLibraryW(LPCWSTR lpLibFileName);
    static HMODULE WINAPI MyLoadLibraryExA(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    static HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    // If this is called, then DX10, DX11 or DX12 will be used to render overlay
    static HRESULT STDMETHODCALLTYPE MyIDXGISwapChain_Present(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags);
    // If any of theses is called, then DX9 will be used to render overlay
    static HRESULT STDMETHODCALLTYPE MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    static HRESULT STDMETHODCALLTYPE MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    // If this is called, then OpenGL 3 will be used to render overlay
    static BOOL WINAPI MywglMakeCurrent(HDC hDC, HGLRC hGLRC);

#elif defined(__linux__)

#endif

public:
    static Hook_Manager& Inst();

    void HookRenderer();

    // Set the found hook and free all other hooks
    void FoundRenderer(Base_Hook *hook);

    inline void AddHook(Base_Hook* hook) { _hooks.push_back(hook); }
};

#endif//NO_OVERLAY

#endif//__INCLUDED_HOOK_BASE_H__