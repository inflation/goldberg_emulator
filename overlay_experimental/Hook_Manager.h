#ifndef __INCLUDED_HOOK_BASE_H__
#define __INCLUDED_HOOK_BASE_H__

#include "Base_Hook.h"

#ifndef NO_OVERLAY

#include <set>

/* 
 *
 */
class Hook_Manager
{
    friend class Base_Hook;

protected:
    // TODO: If needed, create a second vector with only the renderers hook
    // Cause actually, a call to FoundRenderer will unhook everything registered except the renderer hook
    // If you do that, you should consider moving the renderer hooks to its own class and keep this one generic ?
    std::set<Base_Hook*> _hooks; 

    Hook_Manager();
    virtual ~Hook_Manager();

public:
    static Hook_Manager& Inst();

    void AddHook(Base_Hook* hook);
    void RemoveHook(Base_Hook* hook);
};

#endif//NO_OVERLAY

#endif//__INCLUDED_HOOK_BASE_H__