#include "Base_Hook.h"
#include "Hook_Manager.h"

#include <algorithm>

#ifndef NO_OVERLAY
#ifdef STEAM_WIN32

#include "../detours/detours.h"

#define DETOUR_HOOKBEGIN      DetourTransactionBegin
#define DETOUR_UPDATETHREAD() DetourUpdateThread(GetCurrentThread())
#define DETOUR_ENDHOOK        DetourTransactionCommit
#define DETOUR_HOOK           DetourAttach
#define DETOUR_UNHOOK         DetourDetach

#elif defined(__LINUX__)
#include "linux/Linux_Detour.h"

#define DETOUR_HOOKBEGIN      Linux_Detour::transaction_begin
#define DETOUR_UPDATETHREAD() Linux_Detour::update_thread(pthread_self())
#define DETOUR_ENDHOOK        Linux_Detour::transaction_commit
#define DETOUR_HOOK           Linux_Detour::hook_func
#define DETOUR_UNHOOK         Linux_Detour::unhook_func

#endif

Base_Hook::Base_Hook():
    _library(nullptr)
{}

Base_Hook::~Base_Hook()
{
    UnhookAll();
}

const char* Base_Hook::get_lib_name() const
{
    return "<no_name>";
}

void Base_Hook::BeginHook()
{
    DETOUR_HOOKBEGIN();
    DETOUR_UPDATETHREAD();
}

void Base_Hook::EndHook()
{
    DETOUR_ENDHOOK();
}

void Base_Hook::HookFunc(std::pair<void**, void*> hook)
{
    if( DETOUR_HOOK(hook.first, hook.second) == 0 )
        _hooked_funcs.emplace_back(hook);
}

void Base_Hook::UnhookAll()
{
    if (_hooked_funcs.size())
    {
        BeginHook();
        std::for_each(_hooked_funcs.begin(), _hooked_funcs.end(), [](std::pair<void**, void*>& hook) {
            DETOUR_UNHOOK(hook.first, hook.second);
            });
        EndHook();
        _hooked_funcs.clear();
    }
}

#endif//NO_OVERLAY
