#include "vote_manager.hpp"

#include <chaos/vote.h>
#include <ml/abort.h>

#include <new>

#include "../../ml/hw/eeuart.h"
#include "../utils/random.hpp"
#include "../utils/time_conversion.hpp"
#include "chaos_core.hpp"

// Progress bar
// TickBar gVoteTickBar(0, 0, 448, 32);

ChaosVoteManager* gpVoteMan;

ChaosVoteManager::ChaosVoteManager() {
	// Initalize the vote structure where the IRC blob expects it
	pVote = new((void*)VOTE_ADDRESS) ChaosVote();

	// Create a new random vote
	//
	// Ideally we could do this by hooking ai state and doing it there
	// For now this is fine I suppose
	createRandomVote();
}

ChaosVoteManager::~ChaosVoteManager() {
	pVote->~ChaosVote();
	pVote = nil(ChaosVote*);
}

/// Starts a new vote with random effect choices.
void ChaosVoteManager::createRandomVote() {
#ifndef NO_CHAT_VOTING
	pVote->tallyLock.lock();
#endif

	eeUartPuts("ChaosVoteManager::createRandomVote() called");

	// Guess a random tick time
	pVote->nrTicksLeft = chaosSecToTicks(3.f); // chaosSecToTicks(uniformRandomf(25.f, 30.f));

#if 0
	// Get some unique effect IDs from the chaos core
	u32 effectIds[CHAOS_VOTE_COUNT];
	chaosGetCore().getRandomEffectIds(&effectIds[0], CHAOS_VOTE_COUNT);

	for(u32 i = 0; i < CHAOS_VOTE_COUNT; ++i) {
		// Reset vottes
		pVote->voteTallies[i] = 0;
		pVote->effects[i] = effectIds[i];
	}
#endif

#ifdef NO_CHAT_VOTING
	// TODO: Give some random results and bias so that there's at least one winning option
#endif

#ifndef NO_CHAT_VOTING
	pVote->tallyLock.unlock();
#endif
}

/// Called at tick rate to update the vote.
void ChaosVoteManager::update() {
	if(pVote->nrTicksLeft-- == 0) {
		eeUartPuts("Vote ended");
		// Find the winning vote, or tie(s), and enable them.

		// getEffectManager().enableEffect(...);

		// Create a new random vote
		createRandomVote();
	}

	// Update the tick bar
}

void ChaosVoteManager::render() {
	// gVoteTickBar.render();

#if 0
	for(u32 i = 0; i < CHAOS_VOTE_COUNT; ++i) {
	}
#endif
}

void chaosVoteInit() {
	mlASSERT(gpVoteMan == nil(ChaosVoteManager*));
	gpVoteMan = new ChaosVoteManager();
}

void chaosVoteShutdown() {
	mlASSERT(gpVoteMan != nil(ChaosVoteManager*));
	delete gpVoteMan;
	gpVoteMan = nil(ChaosVoteManager*);
}

ChaosVoteManager* chaosVoteMan() {
	return gpVoteMan;
}
