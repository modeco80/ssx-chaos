#include "../ml/hw/eeuart.h"
#include "effect_manager.hpp"
#include "random.hpp"
#include "vote_blob.hpp"

// Since we're not using the C main entry point, we have to call these by hand
extern "C" void __do_global_ctors();
extern "C" void __do_global_dtors();

extern "C" void chaosMain() {
	// call C++ global constructors first
	__do_global_ctors();

	// Initialize systems we need now
	chaos::randomInit();
	chaos::voteInit();

	// TODO: do hooks here

	eeUartPuts("Hello World? Is this thing on?");

	chaos::getEffectManager().enableEffect(0x0, 10000);

	while(true) {
		chaos::getEffectManager().onTick();

		// delay
		for(volatile u32 i = 0; i < 10 * 100000000; ++i);
	}
}
