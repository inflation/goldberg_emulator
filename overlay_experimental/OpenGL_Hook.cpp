#include "../dll/base.h"

#ifndef NO_OVERLAY

#include "OpenGL_Hook.h"
#include "Hook_Manager.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>
#include <impls/imgui_impl_opengl3.h>

#include <GL/glew.h>

#include "steam_overlay.h"

// This is created by OpenGL_Hook::Create, and deleted by the Hook_Manager if not used
static OpenGL_Hook* hook;

void OpenGL_Hook::start_hook()
{
    if (!_hooked)
    {
        Hook_Manager::Inst().FoundRenderer(this);

        GLenum err = glewInit();

        if (err == GLEW_OK)
        {
            _hooked = true;
            PRINT_DEBUG("Hooked OpenGL\n");
            UnhookAll();
            BeginHook();
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)wglSwapBuffers, &OpenGL_Hook::MywglSwapBuffers)
            );
            EndHook();
        }
        else
        {
            PRINT_DEBUG("Failed to hook OpenGL\n");
            /* Problem: glewInit failed, something is seriously wrong. */
            PRINT_DEBUG("Error: %s\n", glewGetErrorString(err));
        }
    }
}

void OpenGL_Hook::resetRenderState()
{
    if (initialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        initialized = false;
    }
}

// Try to make this function and overlay's proc as short as possible or it might affect game's fps.
void OpenGL_Hook::prepareForOverlay(HDC hDC)
{
    HWND hWnd = WindowFromDC(hDC);
    RECT rect;

    GetClientRect(hWnd, &rect);

    if (hWnd != Hook_Manager::Inst().GetOverlay()->GetGameHwnd())
        resetRenderState();

    if (!initialized)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        Hook_Manager::Inst().ChangeGameWindow(hWnd);

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplOpenGL3_Init();

        initialized = true;
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();

    ImGui::NewFrame();

    Hook_Manager::Inst().CallOverlayProc(rect.right, rect.bottom);

    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/////////////////////////////////////////////////////////////////////////////////////
// OpenGL Initialization functions
//BOOL WINAPI OpenGL_Hook::MywglMakeCurrent(HDC hDC, HGLRC hGLRC)
//{
//    auto res = hook->wglMakeCurrent(hDC, hGLRC);
//    hook->hook_ogl();
//    return res;
//}

/////////////////////////////////////////////////////////////////////////////////////
BOOL WINAPI OpenGL_Hook::MywglSwapBuffers(HDC hDC)
{
    hook->prepareForOverlay(hDC);
    return hook->wglSwapBuffers(hDC);
}

OpenGL_Hook::OpenGL_Hook():
    initialized(false),
    wglSwapBuffers(nullptr)
{
    _dll = GetModuleHandle(DLL_NAME);
    // Hook to wglMakeCurrent so we know when it gets called.
    // If its called, then OpenGL will be used to render the overlay.
    //wglMakeCurrent = (decltype(wglMakeCurrent))GetProcAddress(_dll, "wglMakeCurrent");
    //wglSwapBuffers = (decltype(wglSwapBuffers))GetProcAddress(_dll, "wglSwapBuffers");
    //
    //BeginHook();
    //HookFuncs(
    //    std::make_pair<void**, void*>(&(PVOID&)wglMakeCurrent, &OpenGL_Hook::MywglMakeCurrent)
    //);
    //EndHook();
}

OpenGL_Hook::~OpenGL_Hook()
{
    PRINT_DEBUG("OpenGL Hook removed\n");

    if (_hooked)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    hook = nullptr;
}

void OpenGL_Hook::Create()
{
    if (hook == nullptr)
    {
        hook = new OpenGL_Hook;
        hook->start_hook();
        // Register the hook to the Hook Manager
        Hook_Manager::Inst().AddHook(hook);
    }
}

#endif//NO_OVERLAY