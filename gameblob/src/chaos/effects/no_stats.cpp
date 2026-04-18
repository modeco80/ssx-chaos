#include "../effect_manager.hpp"
#include "../effect.hpp"

class NoStatsEffect : public chaos::EffectMixin<0x0> {

    void enable() ml_override {
        // For each player:
        // Copy their ingame BE data into this class
        // Fill the stats with zeros
    }

    void disable() ml_override {
        // For each player:
        // Copy back the stats
    }

};

REGISTER_EFFECT(NoStatsEffect);
