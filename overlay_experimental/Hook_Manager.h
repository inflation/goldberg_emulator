#ifndef __INCLUDED_HOOK_BASE_H__
#define __INCLUDED_HOOK_BASE_H__

#include <Windows.h>
#include <vector>
#include "Base_Hook.h"

class Hook_Manager
{
    friend class Base_Hook;
public:
    using overlayProc_t = void(*)(int,int);
    using hookReady_t = void(*)(void*);

protected:
    std::vector<Base_Hook*> _hooks;

    WNDPROC _gameWndProc;       // The game main windows proc
    HWND _gameHWnd;             // The game main window
    bool _showOverlay;          // Should we render the overlay
    bool _isSet;                // Is the renderer hooked ?
    bool _LoadLibraryHooked;    // Are the LoadLibrary functions hooked ?
    class Steam_Overlay* overlay;

    Hook_Manager();
    virtual ~Hook_Manager();

    void HookLoadLibrary();
    void UnHookLoadLibrary();

public:
    static Hook_Manager& Inst();

    void HookRenderer(Steam_Overlay *overlay);

    // Set the found hook and free all other hooks
    void FoundHook(Base_Hook *hook);

    inline void AddHook(Base_Hook* hook) { _hooks.push_back(hook); }
    void ChangeGameWindow(HWND hWnd) const;
    void CallOverlayProc(int width, int height) const;
    Steam_Overlay* GetOverlay() const { return overlay; }
};


#endif//__INCLUDED_HOOK_BASE_H__