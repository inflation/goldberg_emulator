#ifndef __INCLUDED_X11_HOOK_H__
#define __INCLUDED_X11_HOOK_H__

#include "Base_Hook.h"

#ifdef __LINUX__
#ifndef NO_OVERLAY

#include <X11/X.h> // XEvent types
#include <X11/Xlib.h> // XEvent structure

class X11_Hook : public Base_Hook
{
public:
    static constexpr const char* DLL_NAME = "libX11.so";

private:
    static X11_Hook* _inst;

    // Variables
    bool hooked;
    bool initialized;

    // Functions
    X11_Hook();

    // Hook to X11 window messages
    static int MyXEventsQueued(Display * display, int mode);

    decltype(XEventsQueued)* _XEventsQueued;

    decltype(XPeekEvent)* _XPeekEvent;
    decltype(XNextEvent)* _XNextEvent;
    decltype(XKeysymToKeycode)* _XKeysymToKeycode;
    decltype(XLookupKeysym)* _XLookupKeysym;

public:
    virtual ~X11_Hook();

    void resetRenderState();
    void prepareForOverlay(Display *display);

    bool start_hook();
    static X11_Hook* Inst();
    virtual const char* get_lib_name() const;
};

#endif//NO_OVERLAY
#endif//__LINUX__
#endif//__INCLUDED_X11_HOOK_H__
