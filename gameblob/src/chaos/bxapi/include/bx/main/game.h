#ifndef BX_MAIN_GAME_H
#define BX_MAIN_GAME_H

#include <ml/types.h>

// nodes;
class cAIWorld;
class cPreAI;
class cAI;
class cPostAI;
class cSnowCache;
class cOverlapMan;
class cLessonMan;
class cOverlayMan;
class cFlexPanelMan;
class cParticleMan;

// not nodes
class cWorldConf;
struct tWorldConfEntry;
class cReplayMan;
class cEffectsLoader;
class cCamControl;

class cBxCamera;

class cFont; // see <bx/render/font.h>

// NOTE: Not complete, this just has the bits we want.
class cGame {
   public:
	u32 unk0;
	u32 unk4;

	u32 mVenue;
	cAIWorld* mpAIWorld;
	cPreAI* mpPreAI;
	cAI* mpAI;
	cPostAI* mpPostAI;
	cSnowCache* mpSnowCache;
	cWorldConf* mpWorldConf;
	tWorldConfEntry* mpCurrentWorldConf;
	cOverlapMan* mpOverlapMan;
	cLessonMan* mpLessonMan;
	cOverlayMan* mpOverlayMan;
	cFlexPanelMan* mpFlexPanelMan;
	cReplayMan* mpReplayMan;

	u32 mPause;
	u32 mGameFrames;
	u32 mShouldExit;

	u8 mSkyColor[4];
	float mUnk1[3];

	cFont* mpHUDFont;
	cFont* mpLocaleFont;

	void Update();
	void Render();
};

#endif
