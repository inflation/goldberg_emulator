#include "Base_Hook.h"
#include "Hook_Manager.h"

#include <algorithm>

#ifndef NO_OVERLAY
#ifdef STEAM_WIN32

#include "../detours/detours.h"

void Base_Hook::BeginHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
}

void Base_Hook::EndHook()
{
    DetourTransactionCommit();
}

void Base_Hook::HookFunc(std::pair<void**, void*> hook)
{
    if( DetourAttach(hook.first, hook.second) == 0 )
        _hooked_funcs.emplace_back(hook);
}

void Base_Hook::UnhookAll()
{
    if (_hooked_funcs.size())
    {
        BeginHook();
        std::for_each(_hooked_funcs.begin(), _hooked_funcs.end(), [](std::pair<void**, void*>& hook) {
            DetourDetach(hook.first, hook.second);
            });
        EndHook();
        _hooked_funcs.clear();
    }
}

#else
#include "Linux_Detour.h"

void Base_Hook::BeginHook()
{
    Linux_Detour::transaction_begin();
    Linux_Detour::update_thread(pthread_self());
}

void Base_Hook::EndHook()
{
    Linux_Detour::transaction_commit();
}

void Base_Hook::HookFunc(std::pair<void**, void*> hook)
{
    if( Linux_Detour::hook_func(hook.first, hook.second) == 0 )
        _hooked_funcs.emplace_back(hook);
}

void Base_Hook::UnhookAll()
{
    if (_hooked_funcs.size())
    {
        BeginHook();
        std::for_each(_hooked_funcs.begin(), _hooked_funcs.end(), [](std::pair<void**, void*>& hook) {
            Linux_Detour::unhook_func(hook.first, hook.second);
            });
        EndHook();
        _hooked_funcs.clear();
    }
}

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

#endif//NO_OVERLAY
