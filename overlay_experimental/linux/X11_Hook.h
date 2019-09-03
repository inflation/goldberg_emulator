#ifndef __INCLUDED_X11_HOOK_H__
#define __INCLUDED_X11_HOOK_H__

#include "../Base_Hook.h"

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
    Window game_wnd;

    // Functions
    X11_Hook();

    // Hook to X11 window messages


    decltype(XEventsQueued)* _XEventsQueued;
    decltype(XPeekEvent)* _XPeekEvent;
    decltype(XNextEvent)* _XNextEvent;
    decltype(XPending)* _XPending;
    //decltype(XKeysymToKeycode)* _XKeysymToKeycode;
    //decltype(XLookupKeysym)* _XLookupKeysym;
    //decltype(XGetGeometry)* _XGetGeometry;

public:
    static int MyXEventsQueued(Display * display, int mode);
    static int MyXNextEvent(Display* display, XEvent *event);
    static int MyXPeekEvent(Display* display, XEvent *event);
    static int MyXPending(Display* display);

    virtual ~X11_Hook();

    void resetRenderState();
    void prepareForOverlay(Display *display, Window wnd);

    Window get_game_wnd() const{ return game_wnd; }

    bool start_hook();
    static X11_Hook* Inst();
    virtual const char* get_lib_name() const;

    inline decltype(XEventsQueued)* get_XEventsQueued() const { return _XEventsQueued; }
    inline decltype(XPeekEvent)*    get_XPeekEvent()    const { return _XPeekEvent; }
    inline decltype(XNextEvent)*    get_XNextEvent()    const { return _XNextEvent; }
    inline decltype(XPending)*      get_XPending()      const { return _XPending; }

    inline void loadXEventsQueued(decltype(XEventsQueued)* pfnXEventsQueued) { _XEventsQueued = pfnXEventsQueued; }
    inline void loadXPeekEvent(decltype(XPeekEvent)* pfnXPeekEvent) { _XPeekEvent = pfnXPeekEvent; }
    inline void loadXNextEvent(decltype(XNextEvent)* pfnXNextEvent) { _XNextEvent = pfnXNextEvent; }
    inline void loadXPending(decltype(XPending)* pfnXPending) { _XPending = pfnXPending; }
};

#endif//NO_OVERLAY
#endif//__LINUX__
#endif//__INCLUDED_X11_HOOK_H__
