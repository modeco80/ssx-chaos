#ifndef CHAOS_HOOK_TRAMPOLINE_HPP
#define CHAOS_HOOK_TRAMPOLINE_HPP

#include <ml/types.h>

namespace chaos {

    typedef void* HookHandle;

    void trampolineInit();
    void trampolineShutdown();

    /// Does a trampoline hook.
    /// NOTE: There is no "thiscall" variant, because thiscall on MIPS does not meaningfully alter calling convention.
    /// Therefore, this function can be used to hook class member functions AND regular functions without any issues.
    HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline);

    /// Unhooks a function previously hooked.
    void trampolineUnhook(HookHandle hook);

    // Type-safe C++ templates
    #include "trampoline-typesafe.hpp"
}

#endif
