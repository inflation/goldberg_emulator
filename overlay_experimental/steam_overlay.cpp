#include "steam_overlay.h"

#include <thread>
#include <string>
#include <sstream>
#include <imgui.h>
#include <impls/imgui_impl_win32.h>

#include <numeric> // iota

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static decltype(DispatchMessageA)* _DispatchMessageA = DispatchMessageA;
static decltype(DispatchMessageW)* _DispatchMessageW = DispatchMessageW;

LRESULT CALLBACK Steam_Overlay::sHookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return Hook_Manager::Inst().GetOverlay()->HookWndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Steam_Overlay::HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Is the event is a key press
    if (uMsg == WM_KEYDOWN)
    {
        // Tab is pressed and was not pressed before
        if (wParam == VK_TAB && !(lParam & (1 << 30)))
        {
            // If Left Shift is pressed
            if (GetAsyncKeyState(VK_LSHIFT) & (1 << 15))
                ShowOverlay(!show_overlay);
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

bool Steam_Overlay::IgnoreMsg(const MSG* lpMsg)
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

LRESULT WINAPI Steam_Overlay::MyDispatchMessageA(const MSG* lpMsg)
{
    Steam_Overlay* _this = Hook_Manager::Inst().GetOverlay();
    if (_this->IgnoreMsg(lpMsg))
        return 0;
    return _DispatchMessageA(lpMsg);
}

LRESULT WINAPI Steam_Overlay::MyDispatchMessageW(const MSG* lpMsg)
{
    Steam_Overlay* _this = Hook_Manager::Inst().GetOverlay();
    if (_this->IgnoreMsg(lpMsg))
        return 0;
    return _DispatchMessageW(lpMsg);
}

Steam_Overlay::Steam_Overlay(Settings* settings, SteamCallResults* callback_results, SteamCallBacks* callbacks, RunEveryRunCB* run_every_runcb, Networking *network) :
    settings(settings),
    callback_results(callback_results),
    callbacks(callbacks),
    run_every_runcb(run_every_runcb),
    network(network),
    game_hwnd(NULL),
    game_hwnd_proc(nullptr),
    show_overlay(false),
    is_ready(false),
    notif_position(ENotificationPosition::k_EPositionBottomLeft),
    h_inset(0),
    v_inset(0),
    friend_action(friend_action_none),
    overlay_state_changed(false)
{
    run_every_runcb->add(&Steam_Overlay::steam_overlay_run_every_runcb, this);
}

Steam_Overlay::~Steam_Overlay()
{
    run_every_runcb->remove(&Steam_Overlay::steam_overlay_run_every_runcb, this);
}

void Steam_Overlay::steam_overlay_run_every_runcb(void* object)
{
    Steam_Overlay* _this = reinterpret_cast<Steam_Overlay*>(object);
    _this->RunCallbacks();
}

HWND Steam_Overlay::GetGameHwnd() const
{
    return game_hwnd;
}

bool Steam_Overlay::Ready() const
{
    return is_ready;
}

bool Steam_Overlay::NeedPresent() const
{
    return true;
}

void Steam_Overlay::SetNotificationPosition(ENotificationPosition eNotificationPosition)
{
    notif_position = eNotificationPosition;
}

void Steam_Overlay::SetNotificationInset(int nHorizontalInset, int nVerticalInset)
{
    h_inset = nHorizontalInset;
    v_inset = nVerticalInset;
}

void Steam_Overlay::SetupOverlay()
{
    Hook_Manager::Inst().HookRenderer(this);
}

void Steam_Overlay::HookReady(void* hWnd)
{
    if (game_hwnd != hWnd)
    {
        if (!is_ready) // If this is the first time we are ready, hook the window dispatch message, so we can intercept em and disable mouse.
        {
            window_hooks.BeginHook();
                
            window_hooks.HookFuncs(std::make_pair<void**, void*>(&(PVOID&)_DispatchMessageA, &Steam_Overlay::MyDispatchMessageA),
                                   std::make_pair<void**, void*>(&(PVOID&)_DispatchMessageW, &Steam_Overlay::MyDispatchMessageW)
                                // Add XInput and DirectInput hooks to catch all mouse & controllers input when overlay is on
                                    );

            window_hooks.EndHook();

            is_ready = true;
        }

        if (game_hwnd)
            SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)game_hwnd_proc);

        game_hwnd = (HWND)hWnd;
        game_hwnd_proc = (WNDPROC)SetWindowLongPtr(game_hwnd, GWLP_WNDPROC, (LONG_PTR)&Steam_Overlay::sHookWndProc);
    }
}

// https://niemand.com.ar/2019/01/01/how-to-hook-directx-11-imgui/
// https://github.com/spazzarama/Direct3DHook/blob/master/Capture/Hook
// https://github.com/unknownv2/LinuxDetours

void Steam_Overlay::SetupFriends(const std::vector<Friend>* friends)
{
    this->friends = friends;
}

void Steam_Overlay::OpenOverlayInvite(CSteamID lobbyId)
{
    //this->lobbyId = lobbyId;
    show_overlay = true;
}

void Steam_Overlay::OpenOverlay(const char* pchDialog)
{
    // TODO: Show pages depending on pchDialog
    show_overlay = true;
}

void Steam_Overlay::ShowOverlay(bool state)
{
    static RECT old_clip;

    if (show_overlay == state)
        return;

    show_overlay = state;
    if (show_overlay)
    {
        RECT cliRect, wndRect, clipRect;

        GetClipCursor(&old_clip);
        // The window rectangle has borders and menus counted in the size
        GetWindowRect(game_hwnd, &wndRect);
        // The client rectangle is the window without borders
        GetClientRect(game_hwnd, &cliRect);

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
    }
    else
    {
        ClipCursor(&old_clip);
        ImGui::GetIO().MouseDrawCursor = false;
    }
    overlay_state_changed = true;
}

void Steam_Overlay::OverlayProc( int width, int height )
{
    if (!show_overlay)
        return;

    int friend_size = friends->size();

    // Set the overlay windows to the size of the game window
    ImGui::SetNextWindowPos({ 0,0 });
    ImGui::SetNextWindowSize({ static_cast<float>(width),
                               static_cast<float>(height) });

    bool close = false;
    if (ImGui::Begin("SteamOverlay", &close, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        ImGui::LabelText("", "Username: %s(%llu) playing %u",
            settings->get_local_name(),
            settings->get_local_steam_id().ConvertToUint64(),
            settings->get_local_game_id().AppID());

        ImGui::Spacing();

        ImGui::ListBoxHeader("Friends", friend_size);
        if (friend_size)
        {
            for (int i = 0; i < friend_size; ++i)
            {
                ImGui::PushID(i);
                ImGui::Selectable(friends->at(i).name().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick);
                if (ImGui::BeginPopupContextItem("Friends", 1))
                {
                    if (ImGui::Button("Invite"))
                    {
                        friend_action |= friend_action_invite;
                        friend_to_action = friends->at(i).id();
                    }
                    if (ImGui::Button("Join"))
                    {
                        friend_action |= friend_action_join;
                        friend_to_action = friends->at(i).id();
                    }
                    ImGui::EndPopup();
                }
                else if (ImGui::IsMouseDoubleClicked(0))
                {
                    // Here handle the chat with the user friends->at(i).id()
                }
                ImGui::PopID();
            }
        }
        ImGui::ListBoxFooter();

        //RECT rect;
        //GetWindowRect(game_hwnd, &rect);
        //auto pos = ImGui::GetMousePos();
        //ImGui::LabelText("", "Window pos: %dx%d %dx%d", rect.left, rect.top, rect.right, rect.bottom);
        //ImGui::LabelText("", "Mouse pos: %dx%d", (int)pos.x, (int)pos.y);


        ImGui::End();
    }

    //ImGui::ShowDemoWindow();
}

#include "../dll/dll.h"

void Steam_Overlay::RunCallbacks()
{
    if (overlay_state_changed)
    {
        GameOverlayActivated_t data = { 0 };
        data.m_bActive = show_overlay;
        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));

        overlay_state_changed = false;
    }

    if (friend_action & friend_action_invite)
    {
        Common_Message msg;
        Friend_Messages* friend_messages = new Friend_Messages();
        friend_messages->set_type(Friend_Messages::LOBBY_INVITE);
        friend_messages->set_lobby_id(settings->get_lobby().ConvertToUint64());
        msg.set_allocated_friend_messages(friend_messages);
        msg.set_source_id(settings->get_local_steam_id().ConvertToUint64());
        msg.set_dest_id(friend_to_action);
        network->sendTo(&msg, true);

        friend_action &= ~friend_action_invite;
    }

    if (friend_action & friend_action_join)
    {
        FriendGameInfo_t friend_info = {};
        Steam_Friends* steamFriends = get_steam_client()->steam_friends;
        steamFriends->GetFriendGamePlayed(friend_to_action, &friend_info);
        //get_steam_client()->steam_matchmaking->JoinLobby(friend_info.m_steamIDLobby);

        const char* connect = steamFriends->GetFriendRichPresence(friend_to_action, "connect");
        GameRichPresenceJoinRequested_t data = {};
        data.m_steamIDFriend.SetFromUint64(friend_to_action);
        strncpy(data.m_rgchConnect, connect, k_cchMaxRichPresenceValueLength - 1);
        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));

        
        friend_action &= ~friend_action_join;
    }
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
