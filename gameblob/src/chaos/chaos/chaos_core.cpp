#include "chaos_core.hpp"

#include <ml/abort.h>

#include <ml/cxx/fixedvec.hpp>
#include <ml/cxx/freelist.hpp>
#include <ml/cxx/singleton.hpp>
#include <ml/cxx/vec.hpp>

// Need GrMan.
#include <bx/render/graphicsman.h>

#include "../utils/log.hpp"
#include "../utils/random.hpp"
#include "effect.hpp"
#include "vote_manager.hpp"

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

class ChaosCoreImpl {
	// TODO: registered effects could be a seperate class, for now this is fine

	ml::Vec<RegisteredEffect> registeredEffects;
	ml::FixedVec<ActiveEffectInfo, MAX_EFFECTS_ACTIVE> activeEffectStack;

   public:
	i32 getEffectIdIndex(u32 id) {
		for(u32 i = 0; i < registeredEffects.size(); ++i) {
			if(registeredEffects[i].id == id) {
				return i;
			}
		}
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

	void registerEffect(ChaosEffect* effect) {
		// In release, we just handle this by not actually trying to add the effect.
		mlASSERT(getEffectIdIndex(effect->getId()) == -1 && "DUPLICATE EFFECT ID FIX BROKEN CODE");
		if(getEffectIdIndex(effect->getId()) != -1)
			return;

		chaosLogf(LogInfo, "Registering effect \"%s\"", effect->getName());

		RegisteredEffect registerMe;
		registerMe.id = effect->getId();
		registerMe.pEffect = effect;
		registeredEffects.pushBack(registerMe);
	}

   private:
	class FindEffectPredicate {
		ChaosEffect* pFindEffect;

	   public:
		FindEffectPredicate(ChaosEffect* pEffect)
			: pFindEffect(pEffect) {
		}

		bool operator()(const ActiveEffectInfo& info) const {
			return info.pEffect == pFindEffect;
		}
	};

	class OnFramePredicate {
	   public:
		ml::FixedVec<ChaosEffect*, MAX_EFFECTS_ACTIVE> removeList;

		bool operator()(ActiveEffectInfo& pActiveEffectInfo) {
			pActiveEffectInfo.pEffect->onFrame();

			// Update the tick remaining count, which possibly may result in us
			// removing the effect from the effect list. We defer this to
			// after iteration using a FixedVec<> we store effect pointers into.
			if(pActiveEffectInfo.ticksRemaining-- == 0) {
				removeList.push(pActiveEffectInfo.pEffect);
			}

			return true;
		}
	};

	class PreRenderPredicate {
	   public:
		bool operator()(const ActiveEffectInfo& pActiveEffectInfo) const {
			pActiveEffectInfo.pEffect->onPreRender();
			return true;
		}
	};

	class PostRenderPredicate {
	   public:
		bool operator()(const ActiveEffectInfo& pActiveEffectInfo) const {
			pActiveEffectInfo.pEffect->onPostRender();
			return true;
		}
	};

	u32 getActiveEffectIndex(ChaosEffect* pEffect) {
		FindEffectPredicate finder(pEffect);
		return activeEffectStack.find(finder);
	}

	bool doesEffectExistInStack(ChaosEffect* pEffect) {
		return getActiveEffectIndex(pEffect) != -1;
	}

   public:
	// Effect stack routines

	u32 getActiveEffectIds(u32* pEffectIds) {
		for(u32 i = 0; i < activeEffectStack.size(); ++i)
			*pEffectIds++ = activeEffectStack[i].pEffect->getId();
		return activeEffectStack.size();
	}

	/// Removes an effect from the effect stack.
	void removeFromEffectStack(ChaosEffect* pEffect) {
		// we can't exactly remove from what isn't there yet
		mlASSERT(doesEffectExistInStack(pEffect) == true);
		u32 index = getActiveEffectIndex(pEffect);
		pEffect->disable();
		activeEffectStack.remove(index);
	}

	void addToEffectStack(ChaosEffect* pEffect, u32 tickDuration) {
		mlASSERT(pEffect != nil(ChaosEffect*)); // already checked beforehand

		// Don't duplicate effects
		if(doesEffectExistInStack(pEffect)) {
			return;
		}

		ActiveEffectInfo info;
		info.pEffect = pEffect;
		info.ticksRemaining = tickDuration;

		if(!activeEffectStack.push(info)) {
			// TODO: Find effect with lowest tick count and replace that instead
			ActiveEffectInfo old = activeEffectStack.pop();
			old.pEffect->disable();

			activeEffectStack.push(info);
		}

		info.pEffect->enable();
	}

	void updateEffectStack() {
		OnFramePredicate frameState;
		activeEffectStack.forEach(frameState);

		// After iteration, remove all the effects which expired on this update call.
		for(u32 i = 0; i < frameState.removeList.size(); ++i) {
			// chaosLogf(LogInfo, "Removing effect \"%s\" since it expired", frameState.removeList[i]->getName());
			removeFromEffectStack(frameState.removeList[i]);
		}
	}

	void preRenderEffectStack() {
		PreRenderPredicate preRender;
		activeEffectStack.forEach(preRender);
	}

	void postRenderEffectStack() {
		PostRenderPredicate postRender;
		activeEffectStack.forEach(postRender);
	}
};

namespace {

	ml::PtrSingleton<ChaosCore> gpChaosCore;

} // namespace

ChaosCore& chaosGetCore() {
	return gpChaosCore.get();
}

ChaosCore::ChaosCore() {
	mlASSERT(pImpl == nil(ChaosCoreImpl*));
	pImpl = new ChaosCoreImpl();
}

ChaosCore::~ChaosCore() {
	delete pImpl;
	pImpl = nil(ChaosCoreImpl*);
}

void ChaosCore::registerEffect(ChaosEffect* effect) {
	pImpl->registerEffect(effect);
}

void ChaosCore::getRandomEffectIds(u32* pEffectIds, u32 count) {
	ml::Vec<u32> effectIdUsageStack;

	for(u32 i = 0; i < count; ++i) {
		u32 id = pImpl->getRandomEffectId();

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
				id = pImpl->getRandomEffectId();
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
	ChaosEffect* pEffect = pImpl->getEffectById(id);
	mlASSERT(pEffect);
	chaosLogf(LogInfo, "Adding effect \"%s\" for %d ticks", pEffect->getName(), tickLength);
	pImpl->addToEffectStack(pEffect, tickLength);
}

void ChaosCore::clearEffects() {
	u32 effectIds[MAX_EFFECTS_ACTIVE];
	const u32 count = pImpl->getActiveEffectIds(&effectIds[0]);
	for(u32 i = 0; i < count; ++i)
		pImpl->removeFromEffectStack(pImpl->getEffectById(effectIds[i]));
}

const char* ChaosCore::getEffectName(u32 id) {
	ChaosEffect* pEffect = pImpl->getEffectById(id);
	mlASSERT(pEffect);
	return pEffect->getName();
}

// Standard hooks

void ChaosCore::onTick() {
	chaosVoteMan()->update();
	pImpl->updateEffectStack();
}

void ChaosCore::onPreRender() {
	bIsGameRendering = true;
	// pImpl->preRenderEffectStack();
}

void ChaosCore::onPostRender() {
	// pImpl->postRenderEffectStack();
	// After this any rendering calls are ours, so any hooks to rendering functions
	// that effects may have placed should ideally just call the original.
	bIsGameRendering = false;
}

void ChaosCore::render() {
	// chaosVoteMan()->render();
	//  TODO: Render active effect stack
}
