#include "../effect_manager.hpp"
#include "../effect.hpp"

class NoStatsEffect : public chaos::EffectMixin<0x0> {

    const char* getName() const ml_override { return "Zero Stats"; }

    void enable() ml_override {
        // For each player:
        // Copy their ingame BE data into this class's data
        // Fill the stats with zeros
    }

    void disable() ml_override {
        // For each player:
        // Copy back the stats
    }

    // We don't need any of the standard hooks because the game already
    // should update everything for us as soon as the effect applies.

};

REGISTER_EFFECT(NoStatsEffect);
