#ifndef LINUX_DETOUR_H
#define LINUX_DETOUR_H

#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>

class Linux_Detour
{
    public:
        static int update_thread(pthread_t thread_id);
        static int transaction_begin();
        static int transaction_abort();
        static int transaction_commit();
        static int hook_func(void** ppOriginalFunc, void* _hook);
        static int unhook_func(void** ppOriginalFunc, void* _hook);

    private:
        static int hook_func_abs(void** ppOriginalFunc, void* _hook);
        static int hook_func_rel(void** ppOriginalFunc, void* _hook);

        Linux_Detour() = delete;
        Linux_Detour(Linux_Detour const&) = delete;
        Linux_Detour(Linux_Detour &&) = delete;
        Linux_Detour& operator=(Linux_Detour const&) = delete;
        Linux_Detour& operator=(Linux_Detour &&) = delete;
};

extern "C" void *elf_hook(char const *library_filename, void const *library_address, char const *function_name, void const *substitution_address);

#endif // LINUX_DETOUR_H
