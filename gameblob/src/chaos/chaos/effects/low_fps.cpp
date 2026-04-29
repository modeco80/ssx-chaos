#include <bx/main/application.h>

#include "../../utils/log.hpp"
#include "../../utils/random.hpp"
#include "../chaos_core.hpp"
#include "../effect.hpp"

class LowFpsEffect : public ChaosEffectImpl<0x1> {
	const char* getName() const ml_override {
		return "Low FPS";
	}

	void enable() ml_override {
		chaosLog(LogInfo, "Low FPS Effect enabled");
	}

	void disable() ml_override {
		chaosLog(LogInfo, "Low FPS Effect disabled");
		TheApp.mGameRate = 1.f;
	}

	void onFrame() ml_override {
		TheApp.mGameRate = uniformRandomf(0.25f, 1.f);
	}
};

REGISTER_EFFECT(LowFpsEffect);
