#ifndef BX_MAIN_APPLICATION_H
#define BX_MAIN_APPLICATION_H

#include <ml/types.h>

class cGame; // include <bx/main/game.h> in your code
class cBezierManager;
class cFrontend3D;
class cLoadMan;

// these classes defs are needed to complete cApplication, but
// we dont really care too much about them. they're useful too I guess?

class cVenueDB {
   public:
	int venueCount;
	struct tListEntry {
		char bigFileName[64];
		char venueName[64];
		int venueId;
	};

	tListEntry venues[12];
	int mUnk;
};

class cBigfileMount {
   public:
	u32 mBigStatus;
	char mBigFileName[129];
	u32 mBigHandle;
};

class cApplication {
   public:
	int mVbiHandle;
	void* mLocale2;
	int mLocaleSize;
	char* mpszExe;
	int mLocaleLanguage;
	cVenueDB mVenueDb;
	cBigfileMount mBigMount;
	cBezierManager* mBezMan;
	u32 mVblankCounter;
	float mGameRate;
	float mDelta;
	u32 mShouldFlushPads;

	// Modules
	cGame* mpGame;
	cFrontend3D* mpFE3D;
	cLoadMan* mpLoadMan;

	u32 unk2;
	u32 unk3;

	u32 mShowFPSDisplay;

	cGame* GetGame() const {
		return mpGame;
	}
};

// Singleton app.
extern cApplication TheApp;

#endif
