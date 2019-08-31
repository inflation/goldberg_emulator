#include "X11_Hook.h"
#include "Renderer_Detector.h"
#include "../dll/dll.h"

#ifndef NO_OVERLAY

#include <imgui.h>
#include <impls/imgui_impl_x11.h>

#include <dlfcn.h>
#include <unistd.h>
#include <fstream>

extern int ImGui_ImplX11_EventHandler(XEvent &event);

X11_Hook* X11_Hook::_inst = nullptr;

bool X11_Hook::start_hook()
{
    bool res = true;
    if (!hooked)
    {
        res = false;
        std::ifstream flibrary("/proc/self/maps");
        std::string line;
        while( std::getline(flibrary, line) )
        {
            if( strcasestr(line.c_str(), DLL_NAME) != nullptr )
            {
                _library = dlopen(line.substr(line.find('/')).c_str(), RTLD_NOW);

                if( _library != nullptr )
                {
                    // Hook only this
                    _XEventsQueued = (decltype(_XEventsQueued))dlsym(_library, "XEventsQueued");
                    // Thoses are needed to ignore some events when overlay is on
                    _XPeekEvent = (decltype(_XPeekEvent))dlsym(_library, "XPeekEvent");
                    _XNextEvent = (decltype(_XNextEvent))dlsym(_library, "XNextEvent");
                    _XKeysymToKeycode = (decltype(_XKeysymToKeycode))dlsym(_library, "XKeysymToKeycode");
                    _XLookupKeysym = (decltype(_XLookupKeysym))dlsym(_library, "XLookupKeysym");

                    if( _XEventsQueued != nullptr && _XPeekEvent != nullptr
                     && _XNextEvent != nullptr && _XKeysymToKeycode != nullptr
                     && _XLookupKeysym != nullptr )
                    {
                        PRINT_DEBUG("Hooked X11\n");
                        hooked = true;

                        BeginHook();
                        HookFuncs(
                            std::make_pair<void**, void*>(&(void*&)_XEventsQueued, (void*)&X11_Hook::MyXEventsQueued)
                        );

                        EndHook();
                        break;
                    }
                    dlclose(_library);
                }
            }
        }
    }
    return res;
}

void X11_Hook::resetRenderState()
{
    if (initialized)
    {
        initialized = false;
        ImGui_ImplX11_Shutdown();
    }
}

void X11_Hook::prepareForOverlay(Display *display)
{
    if (!initialized)
    {
        ImGui_ImplX11_Init(display);

        ImGuiIO &io = ImGui::GetIO();
        io.KeyMap[ImGuiKey_Tab]        = _XKeysymToKeycode(display, XK_Tab);
        io.KeyMap[ImGuiKey_LeftArrow]  = _XKeysymToKeycode(display, XK_Left);
        io.KeyMap[ImGuiKey_RightArrow] = _XKeysymToKeycode(display, XK_Right);
        io.KeyMap[ImGuiKey_UpArrow]    = _XKeysymToKeycode(display, XK_Up);
        io.KeyMap[ImGuiKey_DownArrow]  = _XKeysymToKeycode(display, XK_Down);
        io.KeyMap[ImGuiKey_PageUp]     = _XKeysymToKeycode(display, XK_Prior);
        io.KeyMap[ImGuiKey_PageDown]   = _XKeysymToKeycode(display, XK_Next);
        io.KeyMap[ImGuiKey_Home]       = _XKeysymToKeycode(display, XK_Home);
        io.KeyMap[ImGuiKey_End]        = _XKeysymToKeycode(display, XK_End);
        io.KeyMap[ImGuiKey_Insert]     = _XKeysymToKeycode(display, XK_Insert);
        io.KeyMap[ImGuiKey_Delete]     = _XKeysymToKeycode(display, XK_Delete);
        io.KeyMap[ImGuiKey_Backspace]  = _XKeysymToKeycode(display, XK_BackSpace);
        io.KeyMap[ImGuiKey_Space]      = _XKeysymToKeycode(display, XK_space);
        io.KeyMap[ImGuiKey_Enter]      = _XKeysymToKeycode(display, XK_Return);
        io.KeyMap[ImGuiKey_Escape]     = _XKeysymToKeycode(display, XK_Escape);
        io.KeyMap[ImGuiKey_A]          = _XKeysymToKeycode(display, XK_A);
        io.KeyMap[ImGuiKey_C]          = _XKeysymToKeycode(display, XK_C);
        io.KeyMap[ImGuiKey_V]          = _XKeysymToKeycode(display, XK_V);
        io.KeyMap[ImGuiKey_X]          = _XKeysymToKeycode(display, XK_X);
        io.KeyMap[ImGuiKey_Y]          = _XKeysymToKeycode(display, XK_Y);
        io.KeyMap[ImGuiKey_Z]          = _XKeysymToKeycode(display, XK_Z);

        initialized = true;
    }

    ImGui_ImplX11_NewFrame();
}

/////////////////////////////////////////////////////////////////////////////////////
// X11 window hooks
bool IgnoreEvent(XEvent &event)
{
    switch(event.type)
    {
        // Keyboard
        case KeyPress: case KeyRelease:
        // MouseButton
        case ButtonPress: case ButtonRelease:
        // Mouse move
        case MotionNotify:
            return true;
    }
    return false;
}

int X11_Hook::MyXEventsQueued(Display *display, int mode)
{
    static Time prev_time = {};
    X11_Hook* inst = X11_Hook::Inst();
    XEvent event = {};

    int res = inst->_XEventsQueued(display, mode);

    if( res > 0 )
    {
        /// The XPeekEvent function returns the first event from the event queue,
        /// but it does not remove the event from the queue.  If the queue is
        /// empty, XPeekEvent flushes the output buffer and blocks until an event
        /// is received.
        inst->_XPeekEvent(display, &event);
        Steam_Overlay* overlay = get_steam_client()->steam_overlay;
        bool show = overlay->ShowOverlay();
        // Is the event is a key press
        if (event.type == KeyPress)
        {
            // Tab is pressed and was not pressed before
            if (event.xkey.keycode == inst->_XKeysymToKeycode(display, XK_Tab) && event.xkey.state & ShiftMask)
            {
                // if key TAB is help, don't make the overlay flicker :p
                if( event.xkey.time != prev_time)
                {
                    overlay->ShowOverlay(!overlay->ShowOverlay());

                    if (overlay->ShowOverlay())
                        show = true;
                }
            }
        }
        else if(event.type == KeyRelease && event.xkey.keycode == inst->_XKeysymToKeycode(display, XK_Tab))
        {
            prev_time = event.xkey.time;
        }

        if (show)
        {
            ImGui_ImplX11_EventHandler(event);

            if (IgnoreEvent(event))
            {
                inst->_XNextEvent(display, &event);
                res = 0;
            }
        }
    }
    // XEventsQueued returns the num of events available.
    // Usually, games tend to read all events queued before calling again XEventsQueued
    // making us unavailable to intercept undesired events
    return res;
}

/////////////////////////////////////////////////////////////////////////////////////

X11_Hook::X11_Hook() :
    initialized(false),
    hooked(false)
{
    //_library = dlopen(DLL_NAME, RTLD_NOW);
}

X11_Hook::~X11_Hook()
{
    PRINT_DEBUG("X11 Hook removed\n");

    resetRenderState();

    //dlclose(_library);

    _inst = nullptr;
}

X11_Hook* X11_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new X11_Hook;

    return _inst;
}

const char* X11_Hook::get_lib_name() const
{
    return DLL_NAME;
}

#endif//NO_OVERLAY
