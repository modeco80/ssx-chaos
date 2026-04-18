#include "trampoline.hpp"
#include "mips.hpp"

#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/types.h>
#include <ml/cxx/freelist.hpp>

namespace chaos {

    struct HookData {
        void* pTarget;
        u32* pTrampoline;
    };

    namespace {
        // Bump this up if it reallly needs it...
        ml::FreeList<HookData, 32> hookList;

        HookData* allocHook(void* pTarget) {
            HookData* hook = hookList.allocate();
            if(hook)
                return hook;

            // Allocate stuff
            hook->pTrampoline = reinterpret_cast<u32*>(mlMalloc(4 * sizeof(u32)));
            mlASSERT(hook->pTrampoline && "Failed to allocate trampoline buffer");

            hook->pTarget = pTarget;

            mlASSERT(false && "Out of free hooks, increase the freelist size in trampoline.cpp!");
            return nil(HookData*);
        }

        void freeHook(HookData* hook) {
            // Free the trampoline buffer
            mlFree(hook->pTrampoline);

            // Reset fields
            hook->pTrampoline = nil(u32*);
            hook->pTarget = vnil;

            hookList.free(hook);
        }

    }

    HookHandle trampolineHook(void* pTarget, void* pHook, void** ppTrampoline) {
        // Try to allocate a hook structure first. If this fails, then
        // return a nil handle to indicate that we couldn't hook.
        HookData* hook = allocHook(pTarget);
        if(hook == nil(HookData*))
            return nil(HookHandle);

        u32* targetInstructions = reinterpret_cast<u32*>(pTarget);

        // TRAMPOLINE SETUP
        u32* pTrampolineBuf = hook->pTrampoline;

        // Copy the original instructions to the start of the trampoline buffer
        *pTrampolineBuf++ = targetInstructions[0];
        *pTrampolineBuf++ = targetInstructions[1];

        // Encode the jump back to the original function body
        *pTrampolineBuf++ = mipse::j(reinterpret_cast<u32>(reinterpret_cast<u32*>(pTarget) + 2)); // j [pTarget+8] (skip the hook)
        *pTrampolineBuf++ = 0x0; // nop

        // In the target, encode the hook to our code
        *targetInstructions++ = mipse::j(reinterpret_cast<u32>(pHook)); // j [pHook]
        *targetInstructions++ = 0x0; // nop

        // If user wants it now, give them a pointer to the trampoline buffer
        // that they can call if they want to call the original function directly
        if(ppTrampoline) {
            *ppTrampoline = reinterpret_cast<void*>(hook->pTrampoline);
        }

        return reinterpret_cast<HookHandle>(hook);
    }

    void trampolineUnhook(HookHandle hook) {
        if(hook != nil(HookHandle)) {
            HookData* pHook = reinterpret_cast<HookData*>(hook);

            // Copy the original instructions back to the target.
            ml_autovar(pTarget, reinterpret_cast<u32*>(pHook->pTarget));
            pTarget[0] = pHook->pTrampoline[0];
            pTarget[1] = pHook->pTrampoline[1];

            // Free the hook.
            freeHook(pHook);
        }
    }

}
