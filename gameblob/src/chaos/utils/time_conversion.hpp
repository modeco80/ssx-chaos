#ifndef CHAOS_TIMEUTILS_HPP
#define CHAOS_TIMEUTILS_HPP

#include <ml/types.h>

// NOTE: These are outlined so the region-specific nature of them
// doesn't need to be exposed. Not great for binary size, but great for sanity!

/// Returns the amount of ticks needed to repressent a amount of seconds
u32 chaosSecToTicks(float secondTime = 1.0f);

/// Returns the amount of seconds repressented by a tick count.
float chaosTickToSecs(u32 tickCount);

#endif
