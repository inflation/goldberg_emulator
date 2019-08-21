#include "Windows_Hook.h"

#include <imgui.h>
#include <impls/imgui_impl_win32.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "../dll/dll.h"

HWND GetGameWindow()
{
    HWND hWnd = FindWindow(NULL, NULL);
    while (hWnd)
    {
        if (!GetParent(hWnd))
        {
            if (GetModuleHandle(NULL) == (HMODULE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE))
                break;
        }
        hWnd = GetWindow(hWnd, GW_HWNDNEXT);
    }
    if (!hWnd)
        PRINT_DEBUG("Failed to get game window HWND\n");
    return hWnd;
}

bool Windows_Hook::start_hook()
{
    if (!_hooked)
    {
        GetRawInputBuffer = ::GetRawInputBuffer;
        GetRawInputData = ::GetRawInputData;

        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(PVOID&)GetRawInputBuffer, &Windows_Hook::MyGetRawInputBuffer),
            std::make_pair<void**, void*>(&(PVOID&)GetRawInputData  , &Windows_Hook::MyGetRawInputData)
        );
        EndHook();

        _hooked = true;
    }
    return true;
}

void Windows_Hook::resetRenderState()
{
    if (initialized)
    {
        initialized = false;
        SetWindowLongPtr(_game_hwnd, GWLP_WNDPROC, (LONG_PTR)_game_wndproc);
        _game_hwnd = nullptr;
        _game_wndproc = nullptr;
        ImGui_ImplWin32_Shutdown();
    }
}

void Windows_Hook::prepareForOverlay(HWND hWnd)
{
    if (!initialized)
    {
        ImGui_ImplWin32_Init(hWnd);

        _game_hwnd = hWnd;
        _game_wndproc = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&Windows_Hook::HookWndProc);

        initialized = true;
    }

    ImGui_ImplWin32_NewFrame();
}

HWND Windows_Hook::GetGameHwnd() const
{
    return _game_hwnd;
}

WNDPROC Windows_Hook::GetGameWndProc() const
{
    return _game_wndproc;
}

/////////////////////////////////////////////////////////////////////////////////////
// Windows window hooks
bool IgnoreMsg(UINT uMsg)
{
    switch (uMsg)
    {
        // Mouse Events
    case WM_MOUSEMOVE:
    case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
    case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
    case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
    case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
    case WM_XBUTTONUP: case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
    case WM_MOUSEACTIVATE: case WM_MOUSEHOVER: case WM_MOUSELEAVE:
        // Keyboard Events
    case WM_KEYDOWN: case WM_KEYUP:
    case WM_SYSKEYDOWN: case WM_SYSKEYUP: case WM_SYSDEADCHAR:
    case WM_CHAR: case WM_UNICHAR: case WM_DEADCHAR:
        // Raw Input Events
    case WM_INPUT:
        return true;
    }
    return false;
}

LRESULT CALLBACK Windows_Hook::HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Steam_Overlay* overlay = get_steam_client()->steam_overlay;
    bool show = overlay->ShowOverlay();
    // Is the event is a key press
    if (uMsg == WM_KEYDOWN)
    {
        // Tab is pressed and was not pressed before
        if (wParam == VK_TAB && !(lParam & (1 << 30)))
        {
            // If Left Shift is pressed
            if (GetAsyncKeyState(VK_LSHIFT) & (1 << 15))
            {
                overlay->ShowOverlay(!overlay->ShowOverlay());
                if (overlay->ShowOverlay())
                    show = true;
            }
        }
    }

    if (show)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        if (IgnoreMsg(uMsg))
            return 0;
    }

    // Call the overlay window procedure
    return CallWindowProc(Windows_Hook::Inst()._game_wndproc, hWnd, uMsg, wParam, lParam);
}

UINT WINAPI Windows_Hook::MyGetRawInputBuffer(PRAWINPUT pData, PUINT pcbSize, UINT cbSizeHeader)
{
    if (!get_steam_client()->steam_overlay->ShowOverlay())
        return Windows_Hook::Inst().GetRawInputBuffer(pData, pcbSize, cbSizeHeader);

    return -1;
}

UINT WINAPI Windows_Hook::MyGetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader)
{
    if (!get_steam_client()->steam_overlay->ShowOverlay())
        return Windows_Hook::Inst().GetRawInputData(hRawInput, uiCommand, pData, pcbSize, cbSizeHeader);

    return -1;
}
/////////////////////////////////////////////////////////////////////////////////////

Windows_Hook::Windows_Hook() :
    initialized(false),
    _game_hwnd(nullptr),
    _game_wndproc(nullptr),
    GetRawInputBuffer(nullptr),
    GetRawInputData(nullptr)
{

}

Windows_Hook::~Windows_Hook()
{
    PRINT_DEBUG("Windows Hook removed\n");

    resetRenderState();
}

Windows_Hook& Windows_Hook::Inst()
{
    static Windows_Hook _inst;
    return _inst;
}