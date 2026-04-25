#ifndef CHAOS_EFFECT_MANAGER_HPP
#define CHAOS_EFFECT_MANAGER_HPP

#include <ml/types.h>

class ChaosEffect; // We only need a fwd-decl for this

class ChaosCore {
	public:
	/// Registers an effect.
	void registerEffect(ChaosEffect* effect);

	/// Gets a set of random effect IDs. This function will intentionally not repeat effect IDs
	/// so that we don't have to handle that in other places.
	void getRandomEffectIds(u32* pEffectIds, u32 count);

	/// Enables an effect by ID. If too many effects are active, this function
	/// will disable the last effect, and then place the effect in that slot.
	void enableEffect(u32 id, u32 tickLength);

	const char* getEffectName(u32 id);

	void clearEffects();

	// Standard hooks

	void onTick();

	void onPreRender();

	void onPostRender();

	void render();
};

/// Gets the global single instance of the chaos core.
ChaosCore& chaosGetCore();

// Registers an effect. Usually used inside of an effects/*.cpp file to register one.
// This is the only public surface that effects use, so they are implemented in bare
// .cpp files and registered like so so the chaos core can use them. Kind of annoying, but hey!
#define REGISTER_EFFECT(T)                                      \
static struct EffectRegister_##T {                          \
	T* instance;                                            \
	EffectRegister_##T() {                                  \
		instance = new T();                                 \
		chaosGetCore().registerEffect(instance); \
	}                                                       \
															\
	~EffectRegister_##T() {                                 \
		delete instance;                                    \
	}                                                       \
} instance_register__##T;


#endif
