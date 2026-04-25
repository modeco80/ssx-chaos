#ifndef CHAOS_HOOK_FNHOOK_HPP
#define CHAOS_HOOK_FNHOOK_HPP

#include "trampoline.hpp"

#define CHAOS_FUNC_HOOK_DECLARE0(funcName, ret) \
class Hook_##funcName { \
	HookHandle handle;  \
	ret (*original)(); \
	static ret hookImpl(); \
	\
	public: \
		void hook() { \
			handle = trampolineHook(ml::bitCast<void*>(&funcName), (void*)&hookImpl, (void**)&original); \
		} \
} hook_##funcName; \
ret Hook_##funcName::hookImpl()

#define CHAOS_FUNC_HOOK_DECLARE(funcName, ret, ...) \
class Hook_##className##_##funcName { \
	HookHandle handle;  \
	ret (*original)(__VA_ARGS__); \
	static ret hookImpl(__VA_ARGS__); \
	public: \
		void hook() { \
			handle = trampolineHook(ml::bitCast<void*>(&funcName), (void*)&hookImpl, (void**)&original); \
		} \
} hook_##funcName; \
ret Hook_##funcName::hookImpl(__VA_ARGS__)

#endif
