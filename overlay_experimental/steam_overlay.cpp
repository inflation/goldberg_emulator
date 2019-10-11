#include "steam_overlay.h"

#ifndef NO_OVERLAY

#include <thread>
#include <string>
#include <sstream>
#include <cctype>
#include <imgui.h>

#include "../dll/dll.h"

#include "Renderer_Detector.h"

#ifdef __WINDOWS__
#include "windows/Windows_Hook.h"
#endif

#include "notification.h"

void Steam_Overlay::steam_overlay_run_every_runcb(void* object)
{
    Steam_Overlay* _this = reinterpret_cast<Steam_Overlay*>(object);
    _this->RunCallbacks();
}

void Steam_Overlay::steam_overlay_callback(void* object, Common_Message* msg)
{
    Steam_Overlay* _this = reinterpret_cast<Steam_Overlay*>(object);
    _this->Callback(msg);
}

Steam_Overlay::Steam_Overlay(Settings* settings, SteamCallResults* callback_results, SteamCallBacks* callbacks, RunEveryRunCB* run_every_runcb, Networking* network) :
    settings(settings),
    callback_results(callback_results),
    callbacks(callbacks),
    run_every_runcb(run_every_runcb),
    network(network),
    setup_overlay_called(false),
    show_overlay(false),
    is_ready(false),
    notif_position(ENotificationPosition::k_EPositionBottomLeft),
    h_inset(0),
    v_inset(0),
    overlay_state_changed(false)
{
    run_every_runcb->add(&Steam_Overlay::steam_overlay_run_every_runcb, this);
    this->network->setCallback(CALLBACK_ID_STEAM_MESSAGES, settings->get_local_steam_id(), &Steam_Overlay::steam_overlay_callback, this);
}

Steam_Overlay::~Steam_Overlay()
{
    run_every_runcb->remove(&Steam_Overlay::steam_overlay_run_every_runcb, this);
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
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    if (!setup_overlay_called)
    {
        setup_overlay_called = true;
        Renderer_Detector::Inst().find_renderer();
    }
}

void Steam_Overlay::HookReady()
{
    if (!is_ready)
    {
        // TODO: Uncomment this and draw our own cursor (cosmetics)
        //ImGuiIO &io = ImGui::GetIO();
        //io.WantSetMousePos = false;
        //io.MouseDrawCursor = false;
        //io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

        is_ready = true;
    }
}

void Steam_Overlay::OpenOverlayInvite(CSteamID lobbyId)
{
    ShowOverlay(true);
}

void Steam_Overlay::OpenOverlay(const char* pchDialog)
{
    // TODO: Show pages depending on pchDialog
    ShowOverlay(true);
}

bool Steam_Overlay::ShowOverlay() const
{
    return show_overlay;
}

void Steam_Overlay::ShowOverlay(bool state)
{
    if (!Ready() || show_overlay == state)
        return;

#ifdef __WINDOWS__
    static RECT old_clip;

    if (state)
    {
        HWND game_hwnd = Windows_Hook::Inst()->GetGameHwnd();
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

#else

#endif

    show_overlay = state;
    overlay_state_changed = true;
}

void Steam_Overlay::NotifyUser(friend_window_state& friend_state, std::string const& message)
{
    if (!(friend_state.window_state & window_state_show) || !show_overlay)
    {
        friend_state.window_state |= window_state_need_attention;
#ifdef __WINDOWS__
        PlaySound((LPCSTR)notif_invite_wav, NULL, SND_ASYNC | SND_MEMORY);
#endif
        AddNotification(message);
    }
}

void Steam_Overlay::SetLobbyInvite(Friend friendId, uint64 lobbyId)
{
    if (!Ready())
        return;

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    auto i = friends.find(friendId);
    if (i != friends.end())
    {
        auto& frd = i->second;
        frd.lobbyId = lobbyId;
        frd.window_state |= window_state_lobby_invite;
        // Make sure don't have rich presence invite and a lobby invite (it should not happen but who knows)
        frd.window_state &= ~window_state_rich_invite;
        NotifyUser(i->second, i->first.name() + " invited you to join a game");
    }
}

void Steam_Overlay::SetRichInvite(Friend friendId, const char* connect_str)
{
    if (!Ready())
        return;

    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    auto i = friends.find(friendId);
    if (i != friends.end())
    {
        auto& frd = i->second;
        strncpy(frd.connect, connect_str, k_cchMaxRichPresenceValueLength - 1);
        frd.window_state |= window_state_rich_invite;
        // Make sure don't have rich presence invite and a lobby invite (it should not happen but who knows)
        frd.window_state &= ~window_state_lobby_invite;
        NotifyUser(i->second, i->first.name() + " invited you to join a game");
    }
}

void Steam_Overlay::FriendConnect(Friend _friend)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    auto& item = friends[_friend];
    item.window_state = window_state_none;
    memset(item.chat_input, 0, max_chat_len);
}

void Steam_Overlay::FriendDisconnect(Friend _friend)
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);
    auto it = friends.find(_friend);
    if (it != friends.end())
        friends.erase(it);
}

void Steam_Overlay::AddNotification(std::string const& message)
{
    Notification notif;
    notif.message = message;
    notif.start_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch());
    notifications.emplace_back(notif);
}

bool Steam_Overlay::FriendHasLobby(uint64 friend_id)
{
    Steam_Friends* steamFriends = get_steam_client()->steam_friends;

    if( std::string(steamFriends->GetFriendRichPresence(friend_id, "connect")).length() > 0 )
        return true;

    FriendGameInfo_t friend_game_info = {};
    steamFriends->GetFriendGamePlayed(friend_id, &friend_game_info);
    if (friend_game_info.m_steamIDLobby.IsValid())
        return true;

    return false;
}

bool Steam_Overlay::IHaveLobby()
{
    Steam_Friends* steamFriends = get_steam_client()->steam_friends;
    if (std::string(steamFriends->GetFriendRichPresence(settings->get_local_steam_id(), "connect")).length() > 0)
        return true;

    if (settings->get_lobby().IsValid())
        return true;

    return false;
}

void Steam_Overlay::BuildContextMenu(Friend const& frd, friend_window_state& state)
{
    if (ImGui::BeginPopupContextItem("Friends", 1))
    {
        bool close_popup = false;

        if (ImGui::Button("Chat"))
        {
            state.window_state |= window_state_show;
            close_popup = true;
        }
        if (IHaveLobby() && ImGui::Button("Invite"))
        {
            state.window_state |= window_state_invite;
            has_friend_action.push(frd);
            close_popup = true;
        }
        if (FriendHasLobby(frd.id()) && ImGui::Button("Join"))
        {
            state.window_state |= window_state_join;
            has_friend_action.push(frd);
            close_popup = true;
        }

        if( close_popup)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void Steam_Overlay::BuildFriendWindow(Friend const& frd, friend_window_state& state)
{
    if (!(state.window_state & window_state_show))
        return;

    bool show = true;
    bool send_chat_msg = false;

    if (ImGui::Begin(frd.name().c_str(), &show))
    {
        if (state.window_state & window_state_need_attention && ImGui::IsWindowFocused())
        {
            state.window_state &= ~window_state_need_attention;
        }

        // Fill this with the chat box and maybe the invitation
        if (state.window_state & (window_state_lobby_invite | window_state_rich_invite))
        {
            ImGui::LabelText("##label", "%s invited you to join the game.", frd.name().c_str());
            ImGui::SameLine();
            if (ImGui::Button("Accept"))
            {
                this->has_friend_action.push(frd);
            }
            ImGui::SameLine();
            if (ImGui::Button("Refuse"))
            {
                state.window_state &= ~(window_state_lobby_invite | window_state_rich_invite);
            }
        }

        ImGui::PushItemWidth(-1.0f); // Make the chat history widget fill the window
        ImGui::ColoredInputTextMultiline("##chat_history", &state.chat_history[0], state.chat_history.length(), { -1.0f, 0 }, ImGuiInputTextFlags_ReadOnly);
        ImGui::PopItemWidth();

        // TODO: Fix the layout of the chat line + send button.
        // It should be like this: chat input should fill the window size minus send button size (button size is fixed)
        // |------------------------------|
        // | /--------------------------\ |
        // | |                          | |
        // | |       chat history       | |
        // | |                          | |
        // | \--------------------------/ |
        // | [____chat line______] [send] |
        // |------------------------------|
        //
        // And it is like this
        // |------------------------------|
        // | /--------------------------\ |
        // | |                          | |
        // | |       chat history       | |
        // | |                          | |
        // | \--------------------------/ |
        // | [__chat line__] [send]       |
        // |------------------------------|
        if (ImGui::InputText("##chat_line", state.chat_input, max_chat_len, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            send_chat_msg = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Send"))
        {
            send_chat_msg = true;
        }

        if (send_chat_msg)
        {
            if (!(state.window_state & window_state_send_message))
            {
                has_friend_action.push(frd);
                state.window_state |= window_state_send_message;
            }
        }
    }
    // User closed the friend window
    if (!show)
        state.window_state &= ~window_state_show;

    ImGui::End();
}

ImFont *font_default;
ImFont *font_notif;

void Steam_Overlay::BuildNotifications(int width, int height)
{
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    int i = 0;

    int font_size = ImGui::GetFontSize();

    for (auto it = notifications.begin(); it != notifications.end(); ++it, ++i)
    {
        auto elapsed_notif = now - it->start_time;

        if ( elapsed_notif < Notification::fade_in)
        {
            float alpha = Notification::max_alpha * (elapsed_notif.count() / static_cast<float>(Notification::fade_in.count()));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, alpha));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(Notification::r, Notification::g, Notification::b, alpha));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, alpha*2));
        }
        else if ( elapsed_notif > Notification::fade_out_start)
        {
            float alpha = Notification::max_alpha * ((Notification::show_time - elapsed_notif).count() / static_cast<float>(Notification::fade_out.count()));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, alpha));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(Notification::r, Notification::g, Notification::b, alpha));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, alpha*2));
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, Notification::max_alpha));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(Notification::r, Notification::g, Notification::b, Notification::max_alpha));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, Notification::max_alpha*2));
        }
        
        ImGui::SetNextWindowPos(ImVec2((float)width - width * Notification::width, Notification::height * font_size * i ));
        ImGui::SetNextWindowSize(ImVec2( width * Notification::width, Notification::height * font_size ));
        ImGui::Begin(std::to_string(10000+i).c_str(), nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | 
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);

        ImGui::TextWrapped("%s", it->message.c_str());

        ImGui::End();

        ImGui::PopStyleColor(3);
    }
    notifications.erase(std::remove_if(notifications.begin(), notifications.end(), [&now](Notification &item) {
        return (now - item.start_time) > Notification::show_time;
    }), notifications.end());
}

void Steam_Overlay::CreateFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig fontcfg;

    fontcfg.OversampleH = fontcfg.OversampleV = 1;
    fontcfg.PixelSnapH = true;
    fontcfg.GlyphRanges = io.Fonts->GetGlyphRangesDefault();

    fontcfg.SizePixels = std::round(io.DisplaySize.y / 68);
    font_default = io.Fonts->AddFontDefault(&fontcfg);

    fontcfg.SizePixels = std::round(io.DisplaySize.y / 60);
    font_notif = io.Fonts->AddFontDefault(&fontcfg);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0; // Disable round window
}

// Try to make this function as short as possible or it might affect game's fps.
void Steam_Overlay::OverlayProc()
{
    std::lock_guard<std::recursive_mutex> lock(global_mutex);

    if (!Ready())
        return;

    ImGuiIO& io = ImGui::GetIO();

    ImGui::PushFont(font_notif);
    BuildNotifications(io.DisplaySize.x, io.DisplaySize.y);
    ImGui::PopFont();

    if (show_overlay)
    {
        int friend_size = friends.size();

        // Set the overlay windows to the size of the game window
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ static_cast<float>(io.DisplaySize.x),
                                   static_cast<float>(io.DisplaySize.y) });

        ImGui::SetNextWindowBgAlpha(0.50);

        ImGui::PushFont(font_default);

        bool show = true;

        if (ImGui::Begin("SteamOverlay", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus))
        {
            ImGui::LabelText("##label", "Username: %s(%llu) playing %u",
                settings->get_local_name(),
                settings->get_local_steam_id().ConvertToUint64(),
                settings->get_local_game_id().AppID());
            ImGui::SameLine();
            Base_Hook* hook = Renderer_Detector::Inst().get_renderer();
            ImGui::LabelText("##label", "Renderer: %s", (hook == nullptr ? "Unknown" : hook->get_lib_name()));

            ImGui::Spacing();

            ImGui::LabelText("##label", "Friends");
            if (!friends.empty())
            {
                ImGui::ListBoxHeader("##label", friend_size);
                std::for_each(friends.begin(), friends.end(), [this](std::pair<Friend const, friend_window_state> &i)
                {
                    ImGui::PushID(i.first.id());

                    ImGui::Selectable(i.first.name().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick);
                    BuildContextMenu(i.first, i.second);
                    if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
                    {
                        i.second.window_state |= window_state_show;
                    }
                    ImGui::PopID();

                    BuildFriendWindow(i.first, i.second);
                });
                ImGui::ListBoxFooter();
            }
        }
        ImGui::End();

        ImGui::PopFont();

        if (!show)
            ShowOverlay(false);
    }// if(show_overlay)
}

void Steam_Overlay::Callback(Common_Message *msg)
{
    if (msg->has_steam_messages())
    {
        Friend frd;
        frd.set_id(msg->source_id());
        auto friend_info = friends.find(frd);
        if (friend_info != friends.end())
        {
            Steam_Messages const& steam_message = msg->steam_messages();
            // Change color to cyan for friend
            friend_info->second.chat_history.append("\x1""00FFFFFF", 9).append(steam_message.message()).append("\n", 1);
            if (!(friend_info->second.window_state & window_state_show))
            {
                friend_info->second.window_state |= window_state_need_attention;
            }

            NotifyUser(friend_info->second, friend_info->first.name() + " says: " + steam_message.message());
        }
    }
}

void Steam_Overlay::RunCallbacks()
{
    if (overlay_state_changed)
    {
        GameOverlayActivated_t data = { 0 };
        data.m_bActive = show_overlay;
        callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));

        overlay_state_changed = false;
    }

    Steam_Friends* steamFriends = get_steam_client()->steam_friends;
    Steam_Matchmaking* steamMatchmaking = get_steam_client()->steam_matchmaking;

    while (!has_friend_action.empty())
    {
        auto friend_info = friends.find(has_friend_action.front());
        if (friend_info != friends.end())
        {
            uint64 friend_id = friend_info->first.id();
            // The user clicked on "Send"
            if (friend_info->second.window_state & window_state_send_message)
            {
                char* input = friend_info->second.chat_input;
                char* end_input = input + strlen(input);
                char* printable_char = std::find_if(input, end_input, [](char c) {
                    return std::isgraph(c);
                });
                // Check if the message contains something else than blanks
                if (printable_char != end_input)
                {
                    // Handle chat send
                    Common_Message msg;
                    Steam_Messages* steam_messages = new Steam_Messages;
                    steam_messages->set_type(Steam_Messages::FRIEND_CHAT);
                    steam_messages->set_message(friend_info->second.chat_input);
                    msg.set_allocated_steam_messages(steam_messages);
                    msg.set_source_id(settings->get_local_steam_id().ConvertToUint64());
                    msg.set_dest_id(friend_id);
                    network->sendTo(&msg, true);

                    friend_info->second.chat_history.append("\x1""00FF00FF", 9).append(input).append("\n", 1);
                }
                *input = 0; // Reset the input field
                friend_info->second.window_state &= ~window_state_send_message;
            }
            // The user clicked on "Invite"
            if (friend_info->second.window_state & window_state_invite)
            {
                std::string connect = steamFriends->GetFriendRichPresence(settings->get_local_steam_id(), "connect");
                if (connect.length() > 0)
                    steamFriends->InviteUserToGame(friend_id, connect.c_str());
                else if (settings->get_lobby().IsValid())
                    steamMatchmaking->InviteUserToLobby(settings->get_lobby(), friend_id);

                friend_info->second.window_state &= ~window_state_invite;
            }
            // The user clicked on "Join"
            if (friend_info->second.window_state & window_state_join)
            {
                std::string connect = steamFriends->GetFriendRichPresence(friend_id, "connect");
                if (connect.length() > 0)
                {
                    GameRichPresenceJoinRequested_t data = {};
                    data.m_steamIDFriend.SetFromUint64(friend_id);
                    strncpy(data.m_rgchConnect, connect.c_str(), k_cchMaxRichPresenceValueLength - 1);
                    callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));
                }
                else
                {
                    FriendGameInfo_t friend_game_info = {};
                    steamFriends->GetFriendGamePlayed(friend_id, &friend_game_info);
                    if (friend_game_info.m_steamIDLobby.IsValid())
                        steamMatchmaking->JoinLobby(friend_game_info.m_steamIDLobby);
                }

                friend_info->second.window_state &= ~window_state_join;
            }
            // The user got a lobby invite and accepted it
            if (friend_info->second.window_state & window_state_lobby_invite)
            {
                GameLobbyJoinRequested_t data;
                data.m_steamIDLobby.SetFromUint64(friend_info->second.lobbyId);
                data.m_steamIDFriend.SetFromUint64(friend_id);
                callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));

                friend_info->second.window_state &= ~window_state_lobby_invite;
            }
            // The user got a rich presence invite and accepted it
            if (friend_info->second.window_state & window_state_rich_invite)
            {
                GameRichPresenceJoinRequested_t data = {};
                data.m_steamIDFriend.SetFromUint64(friend_id);
                strncpy(data.m_rgchConnect, friend_info->second.connect, k_cchMaxRichPresenceValueLength - 1);
                callbacks->addCBResult(data.k_iCallback, &data, sizeof(data));

                friend_info->second.window_state &= ~window_state_rich_invite;
            }
        }
        has_friend_action.pop();
    }
}

#endif
