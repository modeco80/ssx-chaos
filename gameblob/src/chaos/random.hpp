#ifndef CHAOS_RANDOM_HPP
#define CHAOS_RANDOM_HPP

#include <ml/types.h>

namespace chaos {

	/// Initializes random number generation
	void randomInit();

	/// gets a random number. Yknow
	u32 random32();

	/// Ran
	u32 uniformRandom32(u32 low, u32 high);

	u32 randomBytes(u8* pBytes, u32 count);
} // namespace chaos

#endif
