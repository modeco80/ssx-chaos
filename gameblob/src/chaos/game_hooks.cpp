#include <bx/main/game.h>
#include <bx/render/graphicsman.h>

#include "../ml/hw/eeuart.h"
#include "chaos_core.hpp"
#include "utils/hook/classhook.hpp"
#include "utils/hook/trampoline.hpp"

// our cGame::update hook
CHAOS_CLASS_HOOK_DECLARE0(void, cGame, Update) {
	// Call the ontick handler in the ChaosCore
	// getChaosCore().onTick();
	eeUartPuts("cGame::Update hook called!!");

	hook_cGame_Update.original(klass);
}

CHAOS_CLASS_HOOK_DECLARE0(void, cGame, Render) {
	eeUartPuts("cGame::Render hook called!!");
	// The order here is intentional. onPreRender is called before the game renders anything,
	// and onPostRender is called after the game is done
	// getChaosCore().onPreRender();
	hook_cGame_Render.original(klass);
	// getChaosCore().onPostRender();

	// getChaosCore().render();

	// Because we nopped out cGraphicsMan::Submit in cGame::Render so we can draw more of our own things,
	// we submit after onPostRender() is called, allowing us to draw more things.

	GrMan.Submit();
}

extern u32 cGame_Render_NopPoint[2];

void chaosDoGameHooks() {
	// NOP the call to cGraphicsMan::Submit, so we can add our own things.
	cGame_Render_NopPoint[0] = 0x0;
	cGame_Render_NopPoint[1] = 0x0;

	// Hook functions
	hook_cGame_Update.hook();
	hook_cGame_Render.hook();

	eeUartPuts("Game functions hooked!");
}
