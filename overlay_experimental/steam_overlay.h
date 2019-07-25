#pragma once

#include <thread>
#include <string>
#include <sstream>
#include <imgui.h>
#include <impls/imgui_impl_win32.h>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "base.h"
#include "Hook_Manager.h"

static decltype(DispatchMessageA)* _DispatchMessageA = DispatchMessageA;
static decltype(DispatchMessageW)* _DispatchMessageW = DispatchMessageW;

class Steam_Overlay
{
    Settings* settings;
    SteamCallResults* callback_results;
    SteamCallBacks* callbacks;
    RunEveryRunCB* run_every_runcb;

    HWND game_hwnd;
    WNDPROC game_hwnd_proc;
    bool is_ready;
    bool show_overlay;
    Base_Hook window_hooks;
    ENotificationPosition notif_position;
    int h_inset, v_inset;

    Steam_Overlay(Steam_Overlay const&) = delete;
    Steam_Overlay(Steam_Overlay&&) = delete;
    Steam_Overlay& operator=(Steam_Overlay const&) = delete;
    Steam_Overlay& operator=(Steam_Overlay&&) = delete;

    static LRESULT CALLBACK sHookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { Hook_Manager::Inst().GetOverlay()->HookWndProc(hWnd, uMsg, wParam, lParam); }

    LRESULT HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Is the event is a key press
        if (uMsg == WM_KEYDOWN)
        {
            // Tab is pressed and was not pressed before
            if (wParam == VK_TAB && !(lParam & (1 << 30)))
            {
                static RECT old_clip;
                // If Left Shift is pressed
                if (GetAsyncKeyState(VK_LSHIFT) & (1 << (sizeof(SHORT) * 8 - 1)))
                {
                    show_overlay = !show_overlay;
                }

                if (show_overlay)
                {
                    RECT cliRect, wndRect, clipRect;

                    GetClipCursor(&old_clip);
                    // The window rectangle has borders and menus counted in the size
                    GetWindowRect(hWnd, &wndRect);
                    // The client rectangle is the window without borders
                    GetClientRect(hWnd, &cliRect);

                    clipRect = wndRect; // Init clip rectangle

                    // Get Window width with borders
                    wndRect.right -= wndRect.left;
                    // Get Window height with borders & menus
                    wndRect.bottom -= wndRect.top;
                    // Compute the border width
                    int borderWidth = (wndRect.right - cliRect.right) / 2;
                    // Client top clip is the menu bar width minus bottom border
                    clipRect.top += wndRect.bottom - cliRect.bottom - borderWidth;
                    // Client left clip is the left border minus border width
                    clipRect.left += borderWidth;
                    // Here goes the same for right and bottom
                    clipRect.right -= borderWidth;
                    clipRect.bottom -= borderWidth;

                    ClipCursor(&clipRect);

                    ImGui::GetIO().MouseDrawCursor = true;

                    {
                        GameOverlayActivated_t data = { 0 };
                        data.m_bActive = true;
                        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));
                    }
                }
                else
                {
                    ClipCursor(&old_clip);
                    ImGui::GetIO().MouseDrawCursor = false;

                    {
                        GameOverlayActivated_t data = { 0 };
                        data.m_bActive = false;
                        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));
                    }
                }
            }

        }
        // If we should show the overlay
        if (show_overlay)
        {
            // Call the overlay window procedure
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
            return true;
        }
        // Else call the game window procedure
        return CallWindowProc(game_hwnd_proc, hWnd, uMsg, wParam, lParam);
    }

    bool IgnoreMsg(const MSG* lpMsg)
    {
        if (lpMsg->hwnd == game_hwnd && show_overlay)
        {
            switch (lpMsg->message)
            {
            case WM_MOUSEMOVE:
            case WM_MOUSEWHEEL: case WM_MOUSEHWHEEL:
            case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
            case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
            case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
            case WM_XBUTTONUP: case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
            case WM_KEYDOWN: case WM_KEYUP:
            case WM_SYSKEYDOWN: case WM_SYSKEYUP:
            case WM_CHAR:
                // We ignore theses message in the game windows, but our overlay needs them.
                HookWndProc(lpMsg->hwnd, lpMsg->message, lpMsg->wParam, lpMsg->lParam);
                return true;
            }
        }
        return false;
    }

    static LRESULT WINAPI MyDispatchMessageA(const MSG* lpMsg)
    {
        Steam_Overlay* _this = Hook_Manager::Inst().GetOverlay();
        if (_this->IgnoreMsg(lpMsg))
            return 0;
        return _DispatchMessageA(lpMsg);
    }

    static LRESULT WINAPI MyDispatchMessageW(const MSG* lpMsg)
    {
        Steam_Overlay* _this = Hook_Manager::Inst().GetOverlay();
        if (_this->IgnoreMsg(lpMsg))
            return 0;
        return _DispatchMessageW(lpMsg);
    }

public:
    Steam_Overlay(Settings* settings, SteamCallResults* callback_results, SteamCallBacks* callbacks, RunEveryRunCB* run_every_runcb) :
        settings(settings),
        callback_results(callback_results),
        callbacks(callbacks),
        run_every_runcb(run_every_runcb),
        game_hwnd(NULL),
        game_hwnd_proc(nullptr),
        show_overlay(false),
        is_ready(false)
    {}

    ~Steam_Overlay()
    {}

    bool Ready() const { return is_ready; }
    bool NeedPresent() const { return true; }
    void SetNotificationPosition(ENotificationPosition eNotificationPosition) { notif_position = eNotificationPosition; }
    void SetNotificationInset(int nHorizontalInset, int nVerticalInset) { h_inset = nHorizontalInset, v_inset = nVerticalInset; }
    void SetupOverlay() { Hook_Manager::Inst().HookRenderer(this); }

    void HookReady(void* hWnd)
    {
        if (game_hwnd != hWnd)
        {
            if (!is_ready) // If this is the first time we are ready, hook the window dispatch message, so we can intercept em and disable mouse.
            {
                window_hooks.BeginHook();
                
                window_hooks.HookFuncs(std::make_pair<void**, void*>(&(PVOID&)_DispatchMessageA, &Steam_Overlay::MyDispatchMessageA),
                                       std::make_pair<void**, void*>(&(PVOID&)_DispatchMessageW, &Steam_Overlay::MyDispatchMessageW));

                window_hooks.EndHook();
            }

            is_ready = true;
            if (game_hwnd)
                SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)game_hwnd_proc);

            game_hwnd = (HWND)hWnd;
            game_hwnd_proc = (WNDPROC)SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)&Steam_Overlay::sHookWndProc);
        }
    }

    // https://niemand.com.ar/2019/01/01/how-to-hook-directx-11-imgui/
    // https://github.com/spazzarama/Direct3DHook/blob/master/Capture/Hook

    void OverlayProc( int width, int height )
    {
        static int item = -1;
        static const char* strs[] = {
            "test1",
            "test2",
            "test3",
        };

        if (!show_overlay)
            return;

        // Set the overlay windows to the size of the game window
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ static_cast<float>(width),
                                   static_cast<float>(height) });

        ImGui::Begin("SteamOverlay", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::LabelText("", "Username: %s(%llu) playing %u",
            settings->get_local_name(),
            settings->get_local_steam_id().ConvertToUint64(),
            settings->get_local_game_id().AppID());

        ImGui::Spacing();

        //ImGui::ListBoxHeader("");
        ImGui::ListBox("Friends", &item, strs, (sizeof(strs) / sizeof(*strs)));
        //ImGui::ListBoxFooter();

        ImGui::Spacing();

        RECT rect;

        GetWindowRect(game_hwnd, &rect);

        auto pos = ImGui::GetMousePos();


        ImGui::LabelText("", "Window pos: %dx%d %dx%d", rect.left, rect.top, rect.right, rect.bottom);
        ImGui::LabelText("", "Mouse pos: %dx%d", (int)pos.x, (int)pos.y);

        ImGui::End();

    }

    /*
                        switchstr(args[0])
                        {
                            casestr("get"):
                                sstr << "Steam ID: " << _this->_client->settings_client->get_local_steam_id().ConvertToUint64() << std::endl;
                                sstr << "Steam Server ID: " << _this->_client->settings_server->get_local_steam_id().ConvertToUint64() << std::endl;
                                sstr << "Your lobby: " << _this->_client->settings_client->get_lobby().ConvertToUint64() << std::endl;
                                ovlay.write(sstr.str());
                                sstr.str("");
                            break;

                            casestr("get_id") :
                                sstr << _this->_client->settings_client->get_local_steam_id().ConvertToUint64() << std::endl;
                            ovlay.write(sstr.str());
                            sstr.str("");
                            break;

                            casestr("get_server_id") :
                                sstr << _this->_client->settings_server->get_local_steam_id().ConvertToUint64() << std::endl;
                            ovlay.write(sstr.str());
                            sstr.str("");
                            break;

                            casestr("get_lobby") :
                                sstr << "Your lobby: " << _this->_client->settings_client->get_lobby().ConvertToUint64() << std::endl;
                            ovlay.write(sstr.str());
                            sstr.str("");
                            break;

                            casestr("list_friends") :
                            {
                                ovlay.write(str + "\n");

                                int cnt = steamFriends->GetFriendCount(0);
                                for (int i = 0; i < cnt; ++i)
                                {
                                    CSteamID id = steamFriends->GetFriendByIndex(i, 0);
                                    const char* name = steamFriends->GetFriendPersonaName(id);

                                    FriendGameInfo_t friend_info = {};
                                    steamFriends->GetFriendGamePlayed(id, &friend_info);
                                    sstr << id.ConvertToUint64() << '(' << name << ") is playing: " << friend_info.m_gameID.AppID() << std::endl;
                                    ovlay.write(sstr.str());
                                }
                            }
                            break;
                            casestr("list_games") :
                            {
                                ovlay.write(str + "\n");

                                int cnt = steamFriends->GetFriendCount(0);
                                for (int i = 0; i < cnt; ++i)
                                {
                                    CSteamID id = steamFriends->GetFriendByIndex(i, 0);
                                    const char* name = steamFriends->GetFriendPersonaName(id);

                                    std::string connect = steamFriends->GetFriendRichPresence(id, "connect");
                                    FriendGameInfo_t friend_info = {};
                                    steamFriends->GetFriendGamePlayed(id, &friend_info);

                                    if (connect.length() > 0)
                                    {
                                        sstr << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                    }
                                    else if (friend_info.m_steamIDLobby != k_steamIDNil)
                                    {
                                        connect = "+connect_lobby " + std::to_string(friend_info.m_steamIDLobby.ConvertToUint64());
                                        sstr << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                    }
                                    ovlay.write(sstr.str());
                                    sstr.str("");
                                }
                            }
                            break;

                            casestr("invite_user") :
                            {
                                ovlay.write(str + "\n");

                                if (_this->lobbyID.IsValid())
                                {
                                    if (args.size() == 2)
                                    {
                                        std::string& friendName = args[1];
                                        int cnt = steamFriends->GetFriendCount(0);
                                        for (int i = 0; i < cnt; ++i)
                                        {
                                            CSteamID id = steamFriends->GetFriendByIndex(i, 0);
                                            const char* name = steamFriends->GetFriendPersonaName(id);

                                            if (friendName == name)
                                            {
                                                Common_Message msg;
                                                Friend_Messages* friend_messages = new Friend_Messages();
                                                friend_messages->set_type(Friend_Messages::LOBBY_INVITE);
                                                friend_messages->set_lobby_id(_this->lobbyID.ConvertToUint64());
                                                msg.set_allocated_friend_messages(friend_messages);
                                                msg.set_source_id(_this->_client->settings_client->get_local_steam_id().ConvertToUint64());
                                                msg.set_dest_id(id.ConvertToUint64());
                                                _this->_client->network->sendTo(&msg, true);

                                                sstr << "Invite sent" << std::endl;
                                                ovlay.write(sstr.str());
                                                break;
                                            }
                                        }

                                    }
                                    else
                                    {
                                        sstr << "'invite_user' needs only 1 parameter: friendname" << std::endl;
                                        ovlay.write(sstr.str());
                                    }
                                }
                            }
                            break;

                            casestr("join_game") :
                                if (args.size() == 2)
                                {
                                    ovlay.write(str + "\n");
                                    int cnt = steamFriends->GetFriendCount(0);
                                    for (int i = 0; i < cnt; ++i)
                                    {
                                        CSteamID id = steamFriends->GetFriendByIndex(i, 0);
                                        const char* name = steamFriends->GetFriendPersonaName(id);

                                        std::string connect = steamFriends->GetFriendRichPresence(id, "connect");
                                        FriendGameInfo_t friend_info = {};
                                        steamFriends->GetFriendGamePlayed(id, &friend_info);

                                        if (connect.length() > 0)
                                        {
                                            sstr << "1: " << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                        }
                                        else if (std::to_string(friend_info.m_steamIDLobby.ConvertToUint64()) == args[1])
                                        {
                                            connect = "connect " + std::to_string(friend_info.m_steamIDLobby.ConvertToUint64());
                                            sstr << "2: " << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                            matchMaking->JoinLobby(friend_info.m_steamIDLobby);
                                        }
                                        ovlay.write(sstr.str());
                                        sstr.str("");
                                    }
                                }
                            break;

                            casestr("join_user") :
                                if (args.size() == 2)
                                {
                                    ovlay.write(str + "\n");
                                    int cnt = steamFriends->GetFriendCount(0);
                                    for (int i = 0; i < cnt; ++i)
                                    {
                                        CSteamID id = steamFriends->GetFriendByIndex(i, 0);
                                        const char* name = steamFriends->GetFriendPersonaName(id);

                                        std::string connect = steamFriends->GetFriendRichPresence(id, "connect");
                                        FriendGameInfo_t friend_info = {};
                                        steamFriends->GetFriendGamePlayed(id, &friend_info);

                                        if (connect.length() > 0)
                                        {
                                            sstr << "1: " << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                        }
                                        else if (args[1] == name )
                                        {
                                            connect = "connect " + std::to_string(friend_info.m_steamIDLobby.ConvertToUint64());
                                            sstr << "2: " << friend_info.m_gameID.AppID() << "\t" << name << "\t" << connect << std::endl;
                                            matchMaking->JoinLobby(friend_info.m_steamIDLobby);
                                        }
                                        ovlay.write(sstr.str());
                                        sstr.str("");
                                    }
                                }
                            break;
                        }
                    }
                }
                */


};