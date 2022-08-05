/*
 * Copyright (C) 2019-2020 Nemirtingas
 * This file is part of the ingame overlay project
 *
 * The ingame overlay project is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * The ingame overlay project is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the ingame overlay project; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "X11_Hook.h"

#include <imgui.h>
#include <backends/imgui_impl_x11.h>
#include <System/Library.h>

extern int ImGui_ImplX11_EventHandler(XEvent &event);

constexpr decltype(X11_Hook::DLL_NAME) X11_Hook::DLL_NAME;

X11_Hook* X11_Hook::_inst = nullptr;

bool X11_Hook::StartHook(std::function<bool(bool)>& _key_combination_callback)
{
    if (!_Hooked)
    {
        void* hX11 = System::Library::GetLibraryHandle(DLL_NAME);
        if (hX11 == nullptr)
        {
            SPDLOG_WARN("Failed to hook X11: Cannot find {}", DLL_NAME);
            return false;
        }

        System::Library::Library libX11;
        LibraryName = System::Library::GetLibraryPath(hX11);

        if (!libX11.OpenLibrary(LibraryName, false))
        {
            SPDLOG_WARN("Failed to hook X11: Cannot load {}", LibraryName);
            return false;
        }

        XEventsQueued = libX11.GetSymbol<decltype(::XEventsQueued)>("XEventsQueued");
        XPending = libX11.GetSymbol<decltype(::XPending)>("XPending");

        if (XPending == nullptr || XEventsQueued == nullptr)
        {
            SPDLOG_WARN("Failed to hook X11: Cannot load functions.({}, {})", DLL_NAME, (void*)XEventsQueued, (void*)XPending);
            return false;
        }
        SPDLOG_INFO("Hooked X11");

        _KeyCombinationCallback = std::move(_key_combination_callback);
        _Hooked = true;

        UnhookAll();
        BeginHook();
        HookFuncs(
            std::make_pair<void**, void*>(&(void*&)XEventsQueued, (void*)&X11_Hook::MyXEventsQueued),
            std::make_pair<void**, void*>(&(void*&)XPending, (void*)&X11_Hook::MyXPending)
        );
        EndHook();
    }
    return true;
}

void X11_Hook::ResetRenderState()
{
    if (_Initialized)
    {
        _GameWnd = 0;
        _Initialized = false;
        ImGui_ImplX11_Shutdown();
    }
}

bool X11_Hook::PrepareForOverlay(Display *display, Window wnd)
{
    if(!_Hooked)
        return false;

    if (_GameWnd != wnd)
        ResetRenderState();

    if (!_Initialized)
    {
        ImGui_ImplX11_Init(display, (void*)wnd);
        _GameWnd = wnd;

        _Initialized = true;
    }

    ImGui_ImplX11_NewFrame();

    return true;
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
        // Copy to clipboard request
        case SelectionRequest:
            return true;
    }
    return false;
}

int X11_Hook::_CheckForOverlay(Display *d, int num_events)
{
    static Time prev_time = {};
    X11_Hook* inst = Inst();

    if( inst->_Initialized )
    {
        XEvent event;
        while(num_events)
        {
            bool skip_input = inst->_KeyCombinationCallback(false);

            XPeekEvent(d, &event);
            ImGui_ImplX11_EventHandler(event);

            // Is the event is a key press
            if (event.type == KeyPress)
            {
                // Tab is pressed and was not pressed before
                if (event.xkey.keycode == XKeysymToKeycode(d, XK_Tab) && event.xkey.state & ShiftMask)
                {
                    // if key TAB is held, don't make the overlay flicker :p
                    if (event.xkey.time != prev_time)
                    {
                        skip_input = true;
                        inst->_KeyCombinationCallback(true);
                    }
                }
            }
            else if(event.type == KeyRelease && event.xkey.keycode == XKeysymToKeycode(d, XK_Tab))
            {
                prev_time = event.xkey.time;
            }

            if (!skip_input || !IgnoreEvent(event))
            {
                if(num_events)
                    num_events = 1;
                break;
            }

            XNextEvent(d, &event);
            --num_events;
        }
    }
    return num_events;
}

int X11_Hook::MyXEventsQueued(Display *display, int mode)
{
    X11_Hook* inst = X11_Hook::Inst();

    int res = inst->XEventsQueued(display, mode);

    if( res )
    {
        res = inst->_CheckForOverlay(display, res);
    }

    return res;
}

int X11_Hook::MyXPending(Display* display)
{
    int res = Inst()->XPending(display);

    if( res )
    {
        res = Inst()->_CheckForOverlay(display, res);
    }

    return res;
}

/////////////////////////////////////////////////////////////////////////////////////

X11_Hook::X11_Hook() :
    _Initialized(false),
    _Hooked(false),
    _GameWnd(0),
    XEventsQueued(nullptr),
    XPending(nullptr)
{
}

X11_Hook::~X11_Hook()
{
    SPDLOG_INFO("X11 Hook removed");

    ResetRenderState();

    _inst = nullptr;
}

X11_Hook* X11_Hook::Inst()
{
    if (_inst == nullptr)
        _inst = new X11_Hook;

    return _inst;
}

std::string X11_Hook::GetLibraryName() const
{
    return LibraryName;
}
