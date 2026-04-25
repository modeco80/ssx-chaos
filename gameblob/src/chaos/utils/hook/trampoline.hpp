#ifndef CHAOS_HOOK_TRAMPOLINE_HPP
#define CHAOS_HOOK_TRAMPOLINE_HPP

#include <ml/types.h>

typedef void* HookHandle;

/// Does a trampoline hook.
/// Returns a hook handle, or nil(HookHandle) on error.
///
/// # Notes
/// There is no "thiscall" variant, because thiscall on MIPS does not meaningfully alter calling convention.
/// Therefore, this function can be used to hook class member functions AND regular functions without any issues.
HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline);

/// Unhooks a function previously hooked with [trampolineHook()].
void trampolineUnhook(HookHandle hook);

#endif
