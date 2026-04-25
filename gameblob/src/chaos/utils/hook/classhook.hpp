#ifndef CHAOS_HOOK_CLASSHOOK_HPP
#define CHAOS_HOOK_CLASSHOOK_HPP

// Class hooking helpers.

#include "trampoline.hpp"

// We have to use memcpy() to tell gcc we're type punning...
extern "C" void* memcpy(void* dst, const void* src, u32 len);

#define CHAOS_CLASS_HOOK_DECLARE0(ret, className, funcName) \
class Hook_##className##_##funcName { \
		HookHandle handle;  \
		ret (*original)(className* klass); \
		static ret hookImpl(className* klass); \
		\
	public: \
		void hook() { \
			void* barePointer; \
			ml_autovar(mptr, &className::funcName); \
			memcpy(&barePointer, ((u8*)&mptr + 4), sizeof(void*));  \
			handle = trampolineHook(barePointer, (void*)&hookImpl, (void**)&original); \
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
			void* barePointer; \
			ml_autovar(mptr, &className::funcName); \
			memcpy(&barePointer, ((u8*)&mptr + 4), sizeof(void*));  \
			handle = trampolineHook(barePointer, (void*)&hookImpl, (void**)&original); \
		} \
} hook_##className##_##funcName; \
ret Hook_##className##_##funcName::hookImpl(className* klass, __VA_ARGS__)

#endif
