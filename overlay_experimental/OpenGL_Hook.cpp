#include "OpenGL_Hook.h"
#include "Windows_Hook.h"
#include "Hook_Manager.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_opengl3.h>

#include <GL/glew.h>

#include "steam_overlay.h"

OpenGL_Hook* OpenGL_Hook::_inst = nullptr;

bool OpenGL_Hook::start_hook()
{
    bool res = true;
    if (!_hooked)
    {
        if (!Windows_Hook::Inst().start_hook())
            return false;

        GLenum err = glewInit();

        if (err == GLEW_OK)
        {
            PRINT_DEBUG("Hooked OpenGL\n");

            _hooked = true;
            Hook_Manager::Inst().FoundRenderer(this);

            wglSwapBuffers = (decltype(wglSwapBuffers))GetProcAddress(reinterpret_cast<HMODULE>(_library), "wglSwapBuffers");

            UnhookAll();
            BeginHook();
            HookFuncs(
                std::make_pair<void**, void*>(&(PVOID&)wglSwapBuffers, &OpenGL_Hook::MywglSwapBuffers)
            );
            EndHook();

            get_steam_client()->steam_overlay->HookReady();
        }
        else
        {
            PRINT_DEBUG("Failed to hook OpenGL\n");
            /* Problem: glewInit failed, something is seriously wrong. */
            PRINT_DEBUG("Error: %s\n", glewGetErrorString(err));
            res = false;
        }
    }
    return true;
}

void OpenGL_Hook::resetRenderState()
{
    if (initialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        Windows_Hook::Inst().resetRenderState();
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

    if (hWnd != Windows_Hook::Inst().GetGameHwnd())
        resetRenderState();

    if (!initialized)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;

        ImGui_ImplOpenGL3_Init();

        initialized = true;
    }
    ImGui_ImplOpenGL3_NewFrame();
    Windows_Hook::Inst().prepareForOverlay(hWnd);

    ImGui::NewFrame();

    get_steam_client()->steam_overlay->OverlayProc(rect.right, rect.bottom);

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
    OpenGL_Hook::Inst()->prepareForOverlay(hDC);
    return OpenGL_Hook::Inst()->wglSwapBuffers(hDC);
}

OpenGL_Hook::OpenGL_Hook():
    initialized(false),
    wglSwapBuffers(nullptr)
{
    _library = LoadLibrary(DLL_NAME);
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

    resetRenderState();

    FreeLibrary(reinterpret_cast<HMODULE>(_library));

    _inst = nullptr;
}

OpenGL_Hook* OpenGL_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new OpenGL_Hook;

    return _inst;
}

#endif//NO_OVERLAY