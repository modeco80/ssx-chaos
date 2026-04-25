#ifndef CHAOS_HOOK_CLASSHOOK_HPP
#define CHAOS_HOOK_CLASSHOOK_HPP

// Class hooking helpers.

#include "trampoline.hpp"

#define CHAOS_CLASS_HOOK_DECLARE0(ret, className, funcName) \
class Hook_##className##_##funcName { \
		HookHandle handle;  \
		ret (*original)(className* klass); \
		static ret hookImpl(className* klass); \
		\
	public: \
		void hook() { \
			handle = trampolineHook(ml::bitCast<void*>(&className::funcName), (void*)&hookImpl, (void**)&original); \
		} \
} hook_##className##_##funcName; \
ret Hook_##className##_##funcName::hookImpl(className* klass)

#define CHAOS_CLASS_HOOK_DECLARE(ret, className, funcName, ...) \
class Hook_##className##_##funcName { \
		HookHandle handle;  \
		ret (*original)(className* klass, __VA_ARGS__); \
		static ret hookImpl(className* klass, __VA_ARGS__); \
	public: \
		void hook() { \
			handle = trampolineHook(ml::bitCast<void*>(&className::funcName), (void*)&hookImpl, (void**)&original); \
		} \
} hook_##className##_##funcName; \
ret Hook_##className##_##funcName::hookImpl(className* klass, __VA_ARGS__)

#endif
