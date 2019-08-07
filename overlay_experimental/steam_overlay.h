#ifndef __INCLUDED_STEAM_OVERLAY_H__
#define __INCLUDED_STEAM_OVERLAY_H__

#include "../dll/base.h"
#include "Hook_Manager.h"
#include <map>
#include <queue>

static constexpr size_t max_chat_len = 768;

enum window_state
{
    window_state_none         = 0,
    window_state_show         = 1<<0,
    window_state_invite       = 1<<1,
    window_state_join         = 1<<2,
    window_state_lobby_invite = 1<<3,
    window_state_rich_invite  = 1<<4,
    window_state_send_message = 1<<5,
};

struct friend_window_state
{
    uint8 window_state;
    union // The invitation (if any)
    {
        uint64 lobbyId;
        char connect[k_cchMaxRichPresenceValueLength];
    };
    std::string chat_history;
    char chat_input[max_chat_len];
};

struct Friend_Less
{
    bool operator()(const Friend& lhs, const Friend& rhs) const
    {
        return lhs.id() < rhs.id();
    }
};

class Steam_Overlay
{
    Settings* settings;
    SteamCallResults* callback_results;
    SteamCallBacks* callbacks;
    RunEveryRunCB* run_every_runcb;
    Networking* network;

    // friend id, show client window (to chat and accept invite maybe)
    std::map<Friend, friend_window_state, Friend_Less> friends;

    HWND game_hwnd;
    WNDPROC game_hwnd_proc;
    bool is_ready;
    bool show_overlay;
    Base_Hook window_hooks;
    ENotificationPosition notif_position;
    int h_inset, v_inset;

    // Callback infos
    std::queue<Friend> has_friend_action;
    bool overlay_state_changed;

    Steam_Overlay(Steam_Overlay const&) = delete;
    Steam_Overlay(Steam_Overlay&&) = delete;
    Steam_Overlay& operator=(Steam_Overlay const&) = delete;
    Steam_Overlay& operator=(Steam_Overlay&&) = delete;

    bool IgnoreMsg(UINT uMsg);

    static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static void steam_overlay_run_every_runcb(void* object);
    static void steam_overlay_callback(void* object, Common_Message* msg);

    void Callback(Common_Message* msg);
    void RunCallbacks();

    bool FriendHasLobby(uint64 friend_id);
    bool IHaveLobby();

    // Right click on friend
    void BuildContextMenu(Friend const& frd, friend_window_state &state);
    // Double click on friend
    void BuildFriendWindow(Friend const& frd, friend_window_state &state);

public:
    Steam_Overlay(Settings* settings, SteamCallResults* callback_results, SteamCallBacks* callbacks, RunEveryRunCB* run_every_runcb, Networking *network);

    ~Steam_Overlay();

    HWND GetGameHwnd() const;

    bool Ready() const;

    bool NeedPresent() const;

    void SetNotificationPosition(ENotificationPosition eNotificationPosition);

    void SetNotificationInset(int nHorizontalInset, int nVerticalInset);
    void SetupOverlay();

    void HookReady(void* hWnd);

    void OverlayProc(int width, int height);

    void OpenOverlayInvite(CSteamID lobbyId);
    void OpenOverlay(const char* pchDialog);

    void ShowOverlay(bool state);

    void SetLobbyInvite(Friend friendId, uint64 lobbyId);
    void SetRichInvite(Friend friendId, const char* connect_str);

    void FriendConnect(Friend _friend);
    void FriendDisconnect(Friend _friend);
};

#endif//__INCLUDED_STEAM_OVERLAY_H__
