#ifndef CHAOS_EFFECT_HPP
#define CHAOS_EFFECT_HPP

#include <ml/types.h>

class ChaosEffect {
	public:
	virtual u32 getId() const = 0;
	virtual const char* getName() const = 0;

	/// Enables this effect. Do your setup here.
	virtual void enable() {
	}

	/// Disables this effect. Tear down and restore everything here, if possible.
	virtual void disable() {
	}

	// DEFAULT HOOKS: These are few and far between but are provided because
	// they are useful to many effects.

	/// Called every game tick, do something interesting here.
	virtual void onFrame() {
	}

	/// Called before the game's rendering begins in case something wants to alter render state before
	/// the game does
	virtual void onPreRender() {
	}

	/// Same, but called *after* the game is done.
	virtual void onPostRender() {
	}
};

template <u32 id>
class ChaosEffectImpl : public ChaosEffect {
	public:
	u32 getId() const ml_override {
		return id;
	}
};

#endif
