#ifndef __INCLUDED_OPENGL_HOOK_H__
#define __INCLUDED_OPENGL_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

class OpenGL_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "opengl32.dll";
    
    using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);
    using wglMakeCurrent_t = BOOL(WINAPI*)(HDC, HGLRC);

private:
    // Variables
    bool initialized;

    // Functions
    OpenGL_Hook();
    virtual ~OpenGL_Hook();

    void hook_ogl();
    void resetRenderState();
    void prepareForOverlay(HDC hDC);

    // Hook to render functions
    static BOOL WINAPI MywglSwapBuffers(HDC hDC);

    wglSwapBuffers_t wglSwapBuffers;
    
    // Hook functions so we know we use OGL
    static BOOL WINAPI MywglMakeCurrent(HDC hDC, HGLRC hGLRC);

    wglMakeCurrent_t wglMakeCurrent;

public:
    static void Create(); // Initialize OGL Hook.
};

#endif//NO_OVERLAY
#endif//__INCLUDED_OPENGL_HOOK_H__