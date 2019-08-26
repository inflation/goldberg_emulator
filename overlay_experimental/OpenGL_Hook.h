#ifndef __INCLUDED_OPENGL_HOOK_H__
#define __INCLUDED_OPENGL_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

class OpenGL_Hook : public Base_Hook
{
public:
    static constexpr const char *DLL_NAME = "opengl32.dll";
    
    using wglSwapBuffers_t = BOOL(WINAPI*)(HDC);
    //using wglMakeCurrent_t = BOOL(WINAPI*)(HDC, HGLRC);

private:
    static OpenGL_Hook* _inst;

    // Variables
    bool initialized;

    // Functions
    OpenGL_Hook();
    virtual ~OpenGL_Hook();

    void resetRenderState();
    void prepareForOverlay(HDC hDC);

    // Hook to render functions
    static BOOL WINAPI MywglSwapBuffers(HDC hDC);

    wglSwapBuffers_t wglSwapBuffers;
    
    // Hook functions so we know we use OGL
    //static BOOL WINAPI MywglMakeCurrent(HDC hDC, HGLRC hGLRC);

    //wglMakeCurrent_t wglMakeCurrent;

public:
    bool start_hook();
    static OpenGL_Hook* Inst();
    virtual const char* get_lib_name() const;
};

#endif//NO_OVERLAY
#endif//__INCLUDED_OPENGL_HOOK_H__