#include <bx/main/game.h>
#include <bx/render/graphicsman.h>

#include "../ml/hw/eeuart.h"
#include "chaos_core.hpp"
#include "utils/hook/classhook.hpp"
#include "utils/hook/trampoline.hpp"

// our cGame::update hook
CHAOS_CLASS_HOOK_DECLARE0(void, cGame, Update) {
	// Call the ontick handler in the ChaosCore
	//chaosGetCore().onTick();
	hook_cGame_Update.original(klass);
}

CHAOS_CLASS_HOOK_DECLARE0(void, cGame, Render) {
	// The order here is intentional. onPreRender is called before the game renders anything,
	// and onPostRender is called after the game is done.

	// chaosGetCore().onPreRender();
	hook_cGame_Render.original(klass);
	// chaosGetCore().onPostRender();

	// Because we nopped out cGraphicsMan::Submit in cGame::Render,
	// we call it after onPostRender() is called.

	GrMan.Submit();
}

extern u32 cGame_Render_NopPoint[2];

void chaosDoGameHooks() {
	// Hook functions
	if(!hook_cGame_Update.hook()) {
		eeUartPuts("Could not hook cGame::Update?");
		return;
	}

	if(!hook_cGame_Render.hook()) {
		eeUartPuts("Could not hook cGame::Render?");
		return;
	}

	// If we get here, all of our hooks succeeded.
	// NOP the call to cGraphicsMan::Submit in cGame::Render. We'll submit ourselves.
	cGame_Render_NopPoint[0] = 0x0;
	cGame_Render_NopPoint[1] = 0x0;

	eeUartPuts("Game functions hooked!");
}
