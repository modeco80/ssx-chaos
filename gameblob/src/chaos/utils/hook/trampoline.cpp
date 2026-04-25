#include "trampoline.hpp"

#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/types.h>

#include <ml/cxx/freelist.hpp>

#include "mips.hpp"

/// Per-hook saved data.
struct HookData {
	void* pTarget;
	u32 trampoline[4];
};

namespace {
	// Only bump this up if we reallllly have to :(
	ml::FreeList<HookData, 32> hookList;

	HookData* allocHook(void* pTarget) {
		HookData* hook = hookList.allocate();
		if(hook == nil(HookData*)) {
			mlASSERT(false && "Out of free hooks, increase the freelist size!");
			return nil(HookData*);
		}

		// Allocate stuff
		hook->pTarget = pTarget;
		mlMemSet(&hook->trampoline[0], 0xab, 4*sizeof(u32));
	}

	void freeHook(HookData* hook) {
		hookList.free(hook);
	}

} // namespace

HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline) {
	// Try to allocate a hook structure first. If this fails, then
	// return a nil handle to indicate that we couldn't hook.
	HookData* hook = allocHook(pTarget);
	if(hook == nil(HookData*))
		return nil(HookHandle);

	volatile u32* targetInstructions = reinterpret_cast<u32*>(pTarget);

	// TRAMPOLINE SETUP
	volatile u32* pTrampolineBuf = &hook->trampoline[0];

	// Copy the original instructions to the start of the trampoline buffer
	*pTrampolineBuf++ = targetInstructions[0];
	*pTrampolineBuf++ = targetInstructions[1];

	// Encode the jump back to the original function body
	*pTrampolineBuf++ = mipse::j(reinterpret_cast<u32>(reinterpret_cast<u32*>(pTarget) + 2)); // j [pTarget+8] (skip the hook)
	*pTrampolineBuf++ = 0x0;																  // nop

	// In the target, encode the hook to our code
	*targetInstructions++ = mipse::j(reinterpret_cast<u32>(pHook)); // j [pHook]
	*targetInstructions++ = 0x0;									// nop

	// If user passes a valid pointer, give them a pointer to the trampoline buffer
	// that they can call if they want to call the original function
	if(ppTrampoline) {
		*ppTrampoline = reinterpret_cast<void*>(&hook->trampoline[0]);
	}

	return reinterpret_cast<HookHandle>(hook);
}

void trampolineUnhook(HookHandle hook) {
	if(hook != nil(HookHandle)) {
		HookData* pHook = reinterpret_cast<HookData*>(hook);

		// Copy the original instructions back to the target.
		ml_autovar(pTarget, reinterpret_cast<u32*>(pHook->pTarget));
		pTarget[0] = pHook->trampoline[0];
		pTarget[1] = pHook->trampoline[1];

		// Free the hook.
		freeHook(pHook);
	}
}
