#ifndef __INCLUDED_WINDOWS_HOOK_H__
#define __INCLUDED_WINDOWS_HOOK_H__

#include "Base_Hook.h"
#ifndef NO_OVERLAY

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

class Windows_Hook : public Base_Hook
{
public:
    //static constexpr const char* DLL_NAME = "user32.dll";

private:
    // Variables
    bool initialized;
    HWND _game_hwnd;
    WNDPROC _game_wndproc;

    // Functions
    Windows_Hook();
    virtual ~Windows_Hook();

    // Hook to Windows window messages
    decltype(GetRawInputBuffer)* GetRawInputBuffer;
    decltype(GetRawInputData)* GetRawInputData;

    static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static UINT WINAPI MyGetRawInputBuffer(PRAWINPUT pData, PUINT pcbSize, UINT cbSizeHeader);
    static UINT WINAPI MyGetRawInputData(HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader);

public:
    bool start_hook();
    void resetRenderState();
    void prepareForOverlay(HWND);

    HWND GetGameHwnd() const;
    WNDPROC GetGameWndProc() const;

    static Windows_Hook& Inst();
};

#endif//NO_OVERLAY
#endif//__INCLUDED_WINDOWS_HOOK_H__