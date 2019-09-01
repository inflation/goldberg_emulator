#ifndef LINUX_DETOUR_H
#define LINUX_DETOUR_H

#include <pthread.h>

namespace Linux_Detour
{
    int update_thread(pthread_t thread_id);
    int transaction_begin();
    int transaction_abort();
    int transaction_commit();
    int hook_func(void** ppOriginalFunc, void* _hook);
    int unhook_func(void** ppOriginalFunc, void* _hook);
};

#endif // LINUX_DETOUR_H
