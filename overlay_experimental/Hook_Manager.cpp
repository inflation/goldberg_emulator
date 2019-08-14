#include "../dll/dll.h"
#include "Hook_Manager.h"

#ifndef NO_OVERLAY

#include "../detours/detours.h"

#include "DX12_Hook.h"
#include "DX11_Hook.h"
#include "DX10_Hook.h"
#include "DX9_Hook.h"
#include "OpenGL_Hook.h"

#include <algorithm>

decltype(LoadLibraryA    )* _LoadLibraryA     = LoadLibraryA;
decltype(LoadLibraryW    )* _LoadLibraryW     = LoadLibraryW;
decltype(LoadLibraryExA  )* _LoadLibraryExA   = LoadLibraryExA;
decltype(LoadLibraryExW  )* _LoadLibraryExW   = LoadLibraryExW;

void create_hookA(const char* libname)
{
    if (!_stricmp(libname, "d3d9.dll"))
        DX9_Hook::Create();
    else if (!_stricmp(libname, "d3d10.dll"))
        DX10_Hook::Create();
    else if (!_stricmp(libname, "d3d11.dll"))
        DX11_Hook::Create();
    else if (!_stricmp(libname, "d3d12.dll"))
        DX12_Hook::Create();
    else if (!_stricmp(libname, "opengl32.dll"))
        OpenGL_Hook::Create();
}

void create_hookW(const wchar_t *libname)
{
    if (!_wcsicmp(libname, L"d3d9.dll"))
        DX9_Hook::Create();
    else if (!_wcsicmp(libname, L"d3d10.dll"))
        DX10_Hook::Create();
    else if (!_wcsicmp(libname, L"d3d11.dll"))
        DX11_Hook::Create();
    else if (!_wcsicmp(libname, L"d3d12.dll"))
        DX12_Hook::Create();
    else if (!_wcsicmp(libname, L"opengl32.dll"))
        OpenGL_Hook::Create();
}

HMODULE WINAPI mLoadLibraryA(LPCTSTR lpLibFileName)
{
    auto res = _LoadLibraryA(lpLibFileName);
    create_hookA(lpLibFileName);
    return res;
}

HMODULE WINAPI mLoadLibraryW(LPCWSTR lpLibFileName)
{
    auto res = _LoadLibraryW(lpLibFileName);
    create_hookW(lpLibFileName);
    return res;
}

HMODULE WINAPI mLoadLibraryExA(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    auto res = _LoadLibraryA(lpLibFileName);
    create_hookA(lpLibFileName);
    return res;
}

HMODULE WINAPI mLoadLibraryExW(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags)
{
    auto res = _LoadLibraryExW(lpLibFileName, hFile, dwFlags);
    create_hookW(lpLibFileName);
    return res;
}

Hook_Manager::Hook_Manager():
    _isSet(false),
    _LoadLibraryHooked(false)
{}

Hook_Manager::~Hook_Manager()
{
    for (auto& i : _hooks)
        delete i;
}

Hook_Manager& Hook_Manager::Inst()
{
    static Hook_Manager hook;
    return hook;
}

void Hook_Manager::HookRenderer(Steam_Overlay *ovlay)
{
    overlay = ovlay;
    HookLoadLibrary();
    std::vector<std::string> const libraries = { "opengl32.dll", "d3d12.dll", "d3d11.dll", "d3d10.dll", "d3d9.dll" };
    std::vector<std::string>::const_iterator it = libraries.begin();
    while (it != libraries.end())
    {
        it = std::find_if(it, libraries.end(), [](std::string const& name) {
            auto x = GetModuleHandle(name.c_str());
            if (x != 0 && x != INVALID_HANDLE_VALUE)
                return true;
            return false;
            });

        if (it == libraries.end())
            break;

        create_hookA(it->c_str());
        ++it;
    }
}

void Hook_Manager::UnHookLoadLibrary()
{
    _LoadLibraryHooked = false;
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach((PVOID*)& _LoadLibraryA, mLoadLibraryA);
    DetourDetach((PVOID*)& _LoadLibraryW, mLoadLibraryW);
    DetourDetach((PVOID*)& _LoadLibraryExA, mLoadLibraryExA);
    DetourDetach((PVOID*)& _LoadLibraryExW, mLoadLibraryExW);

    DetourTransactionCommit();
}

void Hook_Manager::HookLoadLibrary()
{
    if (!_isSet && !_LoadLibraryHooked)
    {
        _LoadLibraryHooked = true;

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DetourAttach((PVOID*)& _LoadLibraryA, mLoadLibraryA);
        DetourAttach((PVOID*)& _LoadLibraryW, mLoadLibraryW);
        DetourAttach((PVOID*)& _LoadLibraryExA, mLoadLibraryExA);
        DetourAttach((PVOID*)& _LoadLibraryExW, mLoadLibraryExW);

        DetourTransactionCommit();
    }
}

void Hook_Manager::FoundHook(Base_Hook* hook)
{
    if (!_isSet)
    {
        _isSet = true;

        UnHookLoadLibrary();

        // Remove all hooks that are unused
        _hooks.erase(std::remove_if(_hooks.begin(), _hooks.end(), [&hook](Base_Hook* it_hook) {
            if (hook != it_hook)
            {
                delete it_hook;
                return true;
            }
            return false;
        }), _hooks.end());
    }
}

void Hook_Manager::ChangeGameWindow(HWND hWnd) const
{
    overlay->HookReady(hWnd);
}

void Hook_Manager::CallOverlayProc(int width, int height) const
{
    overlay->OverlayProc(width, height);
}

#endif//NO_OVERLAY