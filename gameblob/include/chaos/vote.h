
#ifdef IN_CHAOS_BLOB
	#include <ml/types.h>
#else
// TODO
#endif

#ifndef NO_CHAT_VOTING
	#include "spinlock.hpp"
#endif

// NOTE: All types here are shared between the two ends.

enum {
	/// Amount of effects that can be voted for.
	CHAOS_VOTE_COUNT = 4,
};

#define VOTE_MAGIC 0xa0a0b001
#define VOTE_DEAD_MAGIC 0xd0d0b001

struct Vote {
	u32 magic;

#ifndef NO_CHAT_VOTING
	/// The end holding this lock gets to write to the vote tallies.
	/// Both ends can atomically read.
	SpinLock tallyLock;
#endif

	/// Number of game ticks (60 hz, so 60 is 1s) left until the vote ends
	u32 nrTicksLeft;

	/// The tallies for the vote.
	u32 voteTallies[CHAOS_VOTE_COUNT];

	/// The effects that are being voted for.
	u32 effects[CHAOS_VOTE_COUNT];

#ifdef IN_CHAOS_BLOB
	Vote() {
		magic = VOTE_MAGIC;
	}

	~Vote() {
		magic = VOTE_DEAD_MAGIC;
	}
#endif
};

/// The address of the vote structure in memory. The IRC end will read to this
/// region of memory
#define VOTE_ADDRESS 0xb0000
