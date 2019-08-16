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

    bool _renderer_found;       // Is the renderer hooked ?
    bool _ogl_hooked;           // wglMakeCurrent is hooked ? (opengl)
    Base_Hook* rendererdetect_hook;
    class Steam_Overlay* overlay;

    Hook_Manager();
    virtual ~Hook_Manager();

    void UnHookAllRendererDetector();
    
    void hook_opengl();

    void HookLoadLibrary();

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    WNDPROC _game_wndproc;      // The game main window proc
    HWND _game_hwnd;            // The game main window
    bool _loadlibrary_hooked;   // Are the LoadLibrary functions hooked ?
    bool _dx9_hooked;           // DX9 Present and PresentEx Hooked ?
    bool _dxgi_hooked;          // DXGI Present is hooked ? (DX10, DX11, DX12)

    void HookDXGIPresent();
    void HookDX9Present();
    void HookwglMakeCurrent();
    void hook_dx9();
    void hook_dx10();
    void hook_dx11();
    void hook_dx12();
    void create_hookA(const char* libname);
    void create_hookW(const wchar_t* libname);

    static HMODULE WINAPI MyLoadLibraryA(LPCTSTR lpLibFileName);
    static HMODULE WINAPI MyLoadLibraryW(LPCWSTR lpLibFileName);
    static HMODULE WINAPI MyLoadLibraryExA(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    static HMODULE WINAPI MyLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
    static HRESULT STDMETHODCALLTYPE MyIDXGISwapChain_Present(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags);
    static HRESULT STDMETHODCALLTYPE MyPresent(IDirect3DDevice9* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
    static HRESULT STDMETHODCALLTYPE MyPresentEx(IDirect3DDevice9Ex* _this, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion, DWORD dwFlags);
    static BOOL WINAPI MywglMakeCurrent(HDC hDC, HGLRC hGLRC);

public:
    void ChangeGameWindow(HWND hWnd) const;

protected:

#elif defined(__linux__)

#endif

public:
    static Hook_Manager& Inst();

    void HookRenderer(Steam_Overlay *overlay);

    // Set the found hook and free all other hooks
    void FoundRenderer(Base_Hook *hook);

    inline void AddHook(Base_Hook* hook) { _hooks.push_back(hook); }

    void CallOverlayProc(int width, int height) const;
    Steam_Overlay* GetOverlay() const { return overlay; }
};

#endif//NO_OVERLAY

#endif//__INCLUDED_HOOK_BASE_H__