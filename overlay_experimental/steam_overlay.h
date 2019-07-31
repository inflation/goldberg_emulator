#ifndef __INCLUDED_STEAM_OVERLAY_H__
#define __INCLUDED_STEAM_OVERLAY_H__

#include "../dll/base.h"
#include "Hook_Manager.h"
#include <vector>

enum friend_action
{
    friend_action_none   = 0,
    friend_action_invite = 1<<0,
    friend_action_join   = 1<<1
};

class Steam_Overlay
{
    Settings* settings;
    SteamCallResults* callback_results;
    SteamCallBacks* callbacks;
    RunEveryRunCB* run_every_runcb;
    Networking* network;

    const std::vector<Friend>* friends;

    HWND game_hwnd;
    WNDPROC game_hwnd_proc;
    bool is_ready;
    bool show_overlay;
    Base_Hook window_hooks;
    ENotificationPosition notif_position;
    int h_inset, v_inset;

    // Callback infos
    uint64 friend_to_action;
    int friend_action;
    bool overlay_state_changed;

    Steam_Overlay(Steam_Overlay const&) = delete;
    Steam_Overlay(Steam_Overlay&&) = delete;
    Steam_Overlay& operator=(Steam_Overlay const&) = delete;
    Steam_Overlay& operator=(Steam_Overlay&&) = delete;

    bool IgnoreMsg(const MSG* lpMsg);
    LRESULT HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK sHookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT WINAPI MyDispatchMessageA(const MSG* lpMsg);
    static LRESULT WINAPI MyDispatchMessageW(const MSG* lpMsg);

    static void steam_overlay_run_every_runcb(void* object);
    void RunCallbacks();

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

    void SetupFriends(const std::vector<Friend>* friends);

    void OpenOverlayInvite(CSteamID lobbyId);
    void OpenOverlay(const char* pchDialog);

    void ShowOverlay(bool state);
};

#endif//__INCLUDED_STEAM_OVERLAY_H__
