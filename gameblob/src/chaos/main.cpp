#include "../ml/hw/eeuart.h"
#include "chaos_core.hpp"
#include "utils/random.hpp"
#include "vote_blob.hpp"

// Since we're not using the C main entry point, we have to call these by hand
extern "C" void __do_global_ctors();
extern "C" void __do_global_dtors();

// see game_hooks.cpp
bool chaosDoGameHooks();

extern "C" void chaosMain() {
	// call C++ global constructors first
	__do_global_ctors();

	// Initialize systems we need now
	randomInit();
	voteInit();
	// chaosGetCore().init() ?

	// Try and Hook game functions once the systems are initalized.
	// If this fails, shut everything down and give up.
	if(!chaosDoGameHooks()) {
		eeUartPuts("Failed to hook game functions. Shutting down.");
		__do_global_dtors();
		return;
	}

	// Once all that has gone smoothly, print some nice text I guess
	eeUartPuts("SSX Chaos Mod initalized");

	// We're all ready to go from here, so we can just let the game
	// get back to work. We'll get calls from things we've hooked later on.
}
