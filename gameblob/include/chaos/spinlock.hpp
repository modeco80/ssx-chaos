#ifndef CHAOS_SPINLOCK_HPP
#define CHAOS_SPINLOCK_HPP

#include <ml/types.h>

namespace chaos {

    /// A very basic spin lock. This is used to synchronize access to data
    /// structures which are shared between the IRC node and the game blob.
    struct SpinLock {
        u32 lockFlag;

        SpinLock() {
            // Start unlocked
            lockFlag = 0;
        }

        ~SpinLock() {
            // Drop unlocked
            lockFlag = 0;
        }

        bool tryLock() {
            if(*((volatile u32*)&lockFlag) != 0)
                return false;

            *((volatile u32*)&lockFlag) = 1;
            return true;
        }

        bool tryUnlock() {
            if(*((volatile u32*)&lockFlag) != 1)
                return false;

            *((volatile u32*)&lockFlag) = 0;
            return true;
        }

        void lock() {
            // Should prolly sleep lol
            while(!tryLock())
                ;
        }

        void unlock() {
            // ditto
            while(!tryUnlock())
                ;
        }
    };

}
#endif
