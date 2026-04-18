#include "effect_manager.hpp"
#include "effect.hpp"

#include <ml/cxx/smallvec.hpp>
#include <ml/cxx/freelist.hpp>
#include <ml/cxx/vec.hpp>
#include <ml/cxx/singleton.hpp>

namespace chaos {

    enum { MAX_EFFECTS_ACTIVE = 8 };


    struct RegisteredEffect {
        u32 id;
        Effect* pEffect;
    };


    struct EffectInfo {
        /// How many ticks until this effect should be disabled and removed
        u32 ticksRemaining;

        /// The effect we are running
        Effect* pEffect;
    };

    // Global data.

    namespace {
        ml::Vec<RegisteredEffect> registeredEffects;
        ml::FreeList<EffectInfo, MAX_EFFECTS_ACTIVE> activeEffectStack;
        ml::PtrSingleton<EffectManager> gEffMan;

        // Registered effects

        i32 getEffectIdIndex(u32 id) {
            for(u32 i = 0; i < registeredEffects.size(); ++i)
                if(registeredEffects[i].id == id)
                    return id;
            return -1;
        }

        bool doesEffectIdExist(i32 id) {
            return getEffectIdIndex(id) != -1;
        }

        Effect* getEffectById(u32 id) {
            i32 index = getEffectIdIndex(id);
            if(id != -1)
                return registeredEffects[index].pEffect;
            else
                // No effect.
                return nil(Effect*);
        }

        // Effect stack routines

        u32 getActiveEffectIds(u32* pEffectIds) {
            // we dont have lambdas so be happy with this
            class Iter {
                u32* pEffectIds;
            public:

                explicit Iter(u32* pEffectIds)
                    : pEffectIds(pEffectIds) {}

                u32* getEndPtr() const { return pEffectIds; }

                bool forEachCb(EffectInfo* pEffectInfo) {
                    *pEffectIds++ = pEffectInfo->pEffect->getId();
                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<Iter*>(user)->forEachCb(pEffectInfo);
                }
            };


            Iter iterState(pEffectIds);
            activeEffectStack.forEachItem(&Iter::forEachCbImpl, &iterState);
            return iterState.getEndPtr() - pEffectIds;
        }

        bool doesEffectIdExistInStack(u32 id) {
            Effect* pEffect = getEffectById(id);
            mlASSERT(pEffect != nil(Effect*) && "How do you even manage this what");

            // we dont have lambdas so be happy with this
            class Iter {
                bool found;
                Effect* pLookingEffect;
            public:

                explicit Iter(Effect* pLookingEffect)
                    :pLookingEffect(pLookingEffect) {

                }

                bool foundEffect() const { return found; }

                bool forEachCb(EffectInfo* pEffectInfo) {
                    if(pEffectInfo->pEffect == pLookingEffect) {
                        found = true;
                        return false;
                    }

                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<Iter*>(user)->forEachCb(pEffectInfo);
                }
            };

            Iter iterState(pEffect);
            activeEffectStack.forEachItem(&Iter::forEachCbImpl, &iterState);

            return iterState.foundEffect();
        }

        void removeFromEffectStack(u32 id) {
            // we can't exactly remove from what isn't there yet
            if(!doesEffectIdExistInStack(id))
                return;

            class FindEffectInfo {
                EffectInfo* pEffectInfo;
                u32 id;
            public:

                explicit FindEffectInfo(u32 id)
                    : id(id) {

                }

                EffectInfo* getInfo() const { return pEffectInfo; }

                bool forEachCb(EffectInfo* effectInfo) {
                    if(effectInfo->pEffect->getId() == id) {
                        pEffectInfo = effectInfo;
                        return false;
                    }

                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<FindEffectInfo*>(user)->forEachCb(pEffectInfo);
                }
            };

            FindEffectInfo iterState(id);
            activeEffectStack.forEachItem(&FindEffectInfo::forEachCbImpl, &iterState);

            EffectInfo* pInfo = iterState.getInfo();

            mlASSERT(pInfo != nil(EffectInfo*) && "okay, you have some shapeshifting memory or something, because theres NO way this can be true");

            // Disable the effect
            pInfo->pEffect->disable();
            activeEffectStack.free(pInfo);
        }


        void addToEffectStack(u32 id, u32 tickDuration) {
            // Don't duplicate effects
            if(doesEffectIdExistInStack(id))
                return;

            EffectInfo info;
            info.pEffect = getEffectById(id);
            info.ticksRemaining = tickDuration;

            ml_autovar(pAllocatedEffect, activeEffectStack.insert(info));
            if(pAllocatedEffect == nil(EffectInfo*)) {

                // disable the last effect, andc then try again
                ml_autovar(pLastEffect, activeEffectStack.lastAllocatedItem());
                mlASSERT(pLastEffect && "this makes no sense what the fuck is going on");
                removeFromEffectStack(pLastEffect->pEffect->getId());

                addToEffectStack(id, tickDuration);
            }
        }

        void updateEffectStack() {
            class OnFrame {
            public:
                bool forEachCb(EffectInfo* pEffectInfo) {
                    pEffectInfo->pEffect->onFrame();

                    if(pEffectInfo->ticksRemaining-- == 0) {
                        removeFromEffectStack(pEffectInfo->pEffect->getId());
                        return false;
                    }

                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<OnFrame*>(user)->forEachCb(pEffectInfo);
                }
            };

            OnFrame iter;
            activeEffectStack.forEachItem(&OnFrame::forEachCbImpl, &iter);
        }

        void preRenderEffectStack() {
            class OnFrame {
            public:
                bool forEachCb(EffectInfo* pEffectInfo) {
                    pEffectInfo->pEffect->onPreRender();
                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<OnFrame*>(user)->forEachCb(pEffectInfo);
                }
            };

            OnFrame iter;
            activeEffectStack.forEachItem(&OnFrame::forEachCbImpl, &iter);
        }

        void postRenderEffectStack() {
            class OnFrame {
            public:
                bool forEachCb(EffectInfo* pEffectInfo) {
                    pEffectInfo->pEffect->onPreRender();
                    return true;
                }

                static bool forEachCbImpl(EffectInfo* pEffectInfo, void* user) {
                    return reinterpret_cast<OnFrame*>(user)->forEachCb(pEffectInfo);
                }
            };

            OnFrame iter;
            activeEffectStack.forEachItem(&OnFrame::forEachCbImpl, &iter);
        }

    }

    EffectManager& getEffectManager() {
        return gEffMan.get();
    }


    void EffectManager::registerEffect(Effect* effect) {
        mlASSERT(!doesEffectIdExist(effect->getId()) && "DUPLICATE EFFECT ID FIX BROKEN CODE");

        RegisteredEffect registerMe;
        registerMe.id = effect->getId();
        registerMe.pEffect = effect;
        registeredEffects.pushBack(registerMe);
    }


    void EffectManager::getRandomEffectIds(u32* pEffectIds, u32 count) {
        ml::Vec<u32> effectIdUsageStack;

        // Algorithm:
        // For I in 0 .. count:
        //   Value = Random value % registeredEffects.size()
        //      Loop
        //          Iterate used stack
        //              If Value found in used stack
        //                  Value = Random value from 0 to max effect id
        //                  Continue loop
        //              *pEffectIds++ = Value;
        //              Push Value to used stack
        //              Break
    }

    void EffectManager::enableEffect(u32 id, u32 tickLength) {
        addToEffectStack(id, tickLength);
    }

    // Standard hooks

    void EffectManager::onTick() {
        // voteUpdate();

        updateEffectStack();
    }

    void EffectManager::onPreRender() {
        preRenderEffectStack();
    }

    void EffectManager::onPostRender() {
        postRenderEffectStack();
    }

}
