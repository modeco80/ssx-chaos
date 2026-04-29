#include "chaos/chaos_core.hpp"
#include "chaos/vote_manager.hpp"
#include "utils/log.hpp"
#include "utils/random.hpp"
#include "utils/time_conversion.hpp"

// see chaos/game_hooks.cpp
bool chaosDoGameHooks();

extern "C" int chaosMain() {
	// Initialize systems we need now
	randomInit();
	chaosVoteInit();
	// chaosGetCore().init()?

	// Try and Hook game functions once the systems are initalized.
	// If this fails, shut everything down and give up.
	if(!chaosDoGameHooks()) {
		chaosLog(LogErr, "Failed to hook game functions. Shutting down.");
		chaosVoteShutdown();
		// Tell the startup code to tear down things.
		return 1;
	}

	// Once all that has gone smoothly, print some nice text I guess
	chaosLog(LogInfo, "SSX Chaos Mod initalized!");

	// TEMP
	chaosGetCore().enableEffect(0x1, chaosSecToTicks(60.f));

	// We're all ready to go from here, so we can just let the game
	// get back to work. We'll get calls from things we've hooked later on.
	return 0;
}
