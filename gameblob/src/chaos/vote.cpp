#include <chaos/vote.h>

#include <new>

#include "chaos_core.hpp"
#include "vote_blob.hpp"

Vote* gpVote;

// Progress bar
// TickBar gVoteTickBar(0, 0, 448, 32);

void voteInit() {
	// Initalize the vote structure at the given address
	gpVote = new((void*)VOTE_ADDRESS) Vote();

	// TODO: do we have to do this?
	voteCreateRandom();
}

void voteShutdown() {
	gpVote->~Vote();
	gpVote = nil(Vote*);
}

/// Starts a new vote with random effect choices.
void voteCreateRandom() {
#ifndef NO_CHAT_VOTING
	gpVote->tallyLock.lock();
#endif

	// Guess a random tick time
	// gpVote->nrTicksLeft = 10 * kSecond + (rand32() * 500 % 25000);

#if 0
	// Get some unique effect IDs from the chaos core
	u32 effectIds[CHAOS_VOTE_COUNT];
	chaosGetCore().getRandomEffectIds(&effectIds[0], CHAOS_VOTE_COUNT);

	for(u32 i = 0; i < CHAOS_VOTE_COUNT; ++i) {
		// Reset vottes
		gpVote->voteTallies[i] = 0;
		gpVote->effects[i] = effectIds[i];
	}
#endif

#ifdef NO_CHAT_VOTING
	// TODO: Give some random results and bias so that there's at least one winning option
#endif

#ifndef NO_CHAT_VOTING
	gpVote->tallyLock.unlock();
#endif
}

/// Called at tick rate to update the vote.
void voteUpdate() {
	if(gpVote->nrTicksLeft-- == 0) {
		// Find the winning vote, or tie(s), and enable them.

		// getEffectManager().enableEffect(...);

		// Create a new random vote
		voteCreateRandom();
	}

	// Update the tick bar
}

void voteRender() {
	// gVoteTickBar.render();

#if 0
	for(u32 i = 0; i < CHAOS_VOTE_COUNT; ++i) {
	}
#endif
}
