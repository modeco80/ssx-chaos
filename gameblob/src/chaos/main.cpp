#include "../ml/hw/eeuart.h"
#include "chaos/chaos_core.hpp"
#include "chaos/vote_manager.hpp"
#include "utils/random.hpp"

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
		eeUartPuts("Failed to hook game functions. Shutting down.");
		chaosVoteShutdown();
		// Tell the startup code to tear down things.
		return 1;
	}

	// Once all that has gone smoothly, print some nice text I guess
	eeUartPuts("SSX Chaos Mod initalized");

	// We're all ready to go from here, so we can just let the game
	// get back to work. We'll get calls from things we've hooked later on.
	return 0;
}
