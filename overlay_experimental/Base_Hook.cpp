#include "Base_Hook.h"

#include <algorithm>
#include "Hook_Manager.h"

#include "../detours/detours.h"

Base_Hook::~Base_Hook()
{
    UnhookAll();
}

void Base_Hook::BeginHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
}

void Base_Hook::EndHook()
{
    DetourTransactionCommit();
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

void Base_Hook::HookFunc(std::pair<void**, void*> hook)
{
    if( DetourAttach(hook.first, hook.second) == 0 )
        _hooked_funcs.emplace_back(hook);
}