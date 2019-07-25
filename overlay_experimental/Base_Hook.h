#ifndef __INCLUDED_BASE_HOOK_H__
#define __INCLUDED_BASE_HOOK_H__

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>

#include <vector>
#include <utility>

class Base_Hook
{
protected:
    std::vector<std::pair<void**, void*>> _hooked_funcs;

    HMODULE _dll;
    bool _hooked;

    Base_Hook(const Base_Hook&) = delete;
    Base_Hook(Base_Hook&&) = delete;
    Base_Hook& operator =(const Base_Hook&) = delete;
    Base_Hook& operator =(Base_Hook&&) = delete;

public:
    Base_Hook() {}
    virtual ~Base_Hook();

    void BeginHook();
    void EndHook();
    void UnhookAll();

    void HookFuncs(std::pair<void**, void*> hook);

    template<typename T, typename ...Args>
    void HookFuncs(std::pair<T*, T> funcs, Args... args)
    {
        _hooked_funcs.emplace_back(std::move(funcs));
        HookFuncs(funcs);
        HookFuncs(args...);
    }
};

#endif//__INCLUDED_BASE_HOOK_H__