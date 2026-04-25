#include "chaos_core.hpp"

#include <ml/abort.h>

#include <ml/cxx/freelist.hpp>
#include <ml/cxx/singleton.hpp>
#include <ml/cxx/vec.hpp>

// Need GrMan.
#include <bx/render/graphicsman.h>

#include "effect.hpp"
#include "utils/random.hpp"
#include "vote_blob.hpp"


enum { MAX_EFFECTS_ACTIVE = 8 };

struct RegisteredEffect {
	u32 id;
	ChaosEffect* pEffect;
};

struct ActiveEffectInfo {
	/// How many ticks until this effect should be disabled and removed
	u32 ticksRemaining;

	/// The effect we are running
	ChaosEffect* pEffect;
};

// Global data.

namespace {
	ml::Vec<RegisteredEffect> registeredEffects;
	ml::FreeList<ActiveEffectInfo, MAX_EFFECTS_ACTIVE> activeEffectStack;
	ChaosCore gChaosCore;

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

	ChaosEffect* getEffectById(u32 id) {
		i32 index = getEffectIdIndex(id);
		if(id != -1)
			return registeredEffects[index].pEffect;
		else
			// No effect.
			return nil(ChaosEffect*);
	}

	u32 getRandomEffectId() {
		return registeredEffects[uniformRandom32(0, registeredEffects.size())].id;
	}

	// Effect stack routines (poor name, but it originally really was going to be a stack.)

	u32 getActiveEffectIds(u32* pEffectIds) {
		// we dont have lambdas so be happy with this
		class Iter {
			u32* pEffectIds;

			public:
			explicit Iter(u32* pEffectIds)
				: pEffectIds(pEffectIds) {
			}

			u32* getEndPtr() const {
				return pEffectIds;
			}

			bool forEachCb(ActiveEffectInfo* pActiveEffectInfo) {
				*pEffectIds++ = pActiveEffectInfo->pEffect->getId();
				return true;
			}

			static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
				return reinterpret_cast<Iter*>(user)->forEachCb(pActiveEffectInfo);
			}
		};

		Iter iterState(pEffectIds);
		activeEffectStack.forEachItem(&Iter::forEachCbImpl, &iterState);
		return iterState.getEndPtr() - pEffectIds;
	}

	bool doesEffectIdExistInStack(u32 id) {
		ChaosEffect* pEffect = getEffectById(id);

		// Well, we can't confirm something we don't have exists
		if(pEffect != nil(ChaosEffect*))
			return false;

		// we dont have lambdas so be happy with this
		class Iter {
			bool found;
			ChaosEffect* pLookingEffect;

			public:
			explicit Iter(ChaosEffect* pLookingEffect)
				: pLookingEffect(pLookingEffect) {
			}

			bool foundEffect() const {
				return found;
			}

			bool forEachCb(ActiveEffectInfo* pActiveEffectInfo) {
				if(pActiveEffectInfo->pEffect == pLookingEffect) {
					found = true;
					return false;
				}

				return true;
			}

			static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
				return reinterpret_cast<Iter*>(user)->forEachCb(pActiveEffectInfo);
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

		class FindActiveEffectInfo {
			ActiveEffectInfo* pActiveEffectInfo;
			u32 id;

			public:
			explicit FindActiveEffectInfo(u32 id)
				: id(id) {
			}

			ActiveEffectInfo* getInfo() const {
				return pActiveEffectInfo;
			}

			bool forEachCb(ActiveEffectInfo* effectInfo) {
				if(effectInfo->pEffect->getId() == id) {
					pActiveEffectInfo = effectInfo;
					return false;
				}

				return true;
			}

			static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
				return reinterpret_cast<FindActiveEffectInfo*>(user)->forEachCb(pActiveEffectInfo);
			}
		};

		FindActiveEffectInfo iterState(id);
		activeEffectStack.forEachItem(&FindActiveEffectInfo::forEachCbImpl, &iterState);

		ActiveEffectInfo* pInfo = iterState.getInfo();
		mlASSERT(pInfo != nil(ActiveEffectInfo*));

		// Disable the effect
		pInfo->pEffect->disable();
		activeEffectStack.free(pInfo);
	}

	void addToEffectStack(u32 id, u32 tickDuration) {
		// Don't duplicate effects
		if(doesEffectIdExistInStack(id))
			return;

		ActiveEffectInfo info;
		info.pEffect = getEffectById(id);
		info.ticksRemaining = tickDuration;

		ml_autovar(pAllocatedEffect, activeEffectStack.insert(info));
		if(pAllocatedEffect == nil(ActiveEffectInfo*)) {
			// remove the last effect, and then recurse (try again) which should hopefully
			// only occur once
			ml_autovar(pLastEffect, activeEffectStack.lastAllocatedItem());
			removeFromEffectStack(pLastEffect->pEffect->getId());

			addToEffectStack(id, tickDuration);
		}
	}

	// tickers

	class OnFrame {
		public:
		bool forEachCb(ActiveEffectInfo* pActiveEffectInfo) {
			pActiveEffectInfo->pEffect->onFrame();

			// Update the tick count, which possibly may result in us
			// removing the effect from the effect list.
			if(pActiveEffectInfo->ticksRemaining-- == 0) {
				removeFromEffectStack(pActiveEffectInfo->pEffect->getId());
			}

			return true;
		}

		static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
			return reinterpret_cast<OnFrame*>(user)->forEachCb(pActiveEffectInfo);
		}
	};

	class PreRender {
		public:
		bool forEachCb(ActiveEffectInfo* pActiveEffectInfo) {
			pActiveEffectInfo->pEffect->onPreRender();
			return true;
		}

		static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
			return reinterpret_cast<PreRender*>(user)->forEachCb(pActiveEffectInfo);
		}
	};

	class PostRender {
		public:
		bool forEachCb(ActiveEffectInfo* pActiveEffectInfo) {
			pActiveEffectInfo->pEffect->onPostRender();
			return true;
		}

		static bool forEachCbImpl(ActiveEffectInfo* pActiveEffectInfo, void* user) {
			return reinterpret_cast<PostRender*>(user)->forEachCb(pActiveEffectInfo);
		}
	};

	void updateEffectStack() {
		OnFrame onFrame;
		activeEffectStack.forEachItem(onFrame);
	}

	void preRenderEffectStack() {
		PreRender preRender;
		activeEffectStack.forEachItem(preRender);
	}

	void postRenderEffectStack() {
		PostRender postRender;
		activeEffectStack.forEachItem(postRender);
	}

} // namespace

ChaosCore& chaosGetCore() {
	return gChaosCore;
}

void ChaosCore::registerEffect(ChaosEffect* effect) {
	// In release, we just handle this by not actually trying to add the effect id.
	mlASSERT(!doesEffectIdExist(effect->getId()) && "DUPLICATE EFFECT ID FIX BROKEN CODE");
	if(doesEffectIdExist(effect->getId()))
		return;

	RegisteredEffect registerMe;
	registerMe.id = effect->getId();
	registerMe.pEffect = effect;
	registeredEffects.pushBack(registerMe);
}

void ChaosCore::getRandomEffectIds(u32* pEffectIds, u32 count) {
	ml::Vec<u32> effectIdUsageStack;

	for(u32 i = 0; i < count; ++i) {
		u32 id = getRandomEffectId();

#if 0 // FOR LATER (once we have more effects)
	// Make sure the effect id hasn't been used before by looking in the effect stack.
	// If it has been used, then generate a new random id and check again. The loop will
	// break out when we eventually find an ID which hasn't been used yet
		while(true) {
			bool found = false;

			for(u32 j = 0; j < effectIdUsageStack.size(); ++i) {
				if(effectIdUsageStack[j] == id) {
					found = true;
					break;
				}
			}

			if(found) {
				id = getRandomEffectId();
				// we loop back and try again
			} else {
				effectIdUsageStack.pushBack(id);
				break;
			}
		}
#endif

		*pEffectIds++ = id;
	}
}

void ChaosCore::enableEffect(u32 id, u32 tickLength) {
	if(!doesEffectIdExist(id))
		return;
	addToEffectStack(id, tickLength);
}

void ChaosCore::clearEffects() {
	u32 effectIds[MAX_EFFECTS_ACTIVE];
	const u32 count = getActiveEffectIds(&effectIds[0]);
	for(u32 i = 0; i < count; ++i)
		removeFromEffectStack(effectIds[i]);
}

const char* ChaosCore::getEffectName(u32 id) {
	ChaosEffect* pEffect = getEffectById(id);
	mlASSERT(pEffect);
	return pEffect->getName();
}

// Standard hooks

void ChaosCore::onTick() {
	voteUpdate();
	updateEffectStack();
}

void ChaosCore::onPreRender() {
	preRenderEffectStack();
}

void ChaosCore::onPostRender() {
	postRenderEffectStack();
}

void ChaosCore::render() {
	// voteRender();
	// TODO: Render active effect stack
}

