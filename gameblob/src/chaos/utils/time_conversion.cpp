#include "time_conversion.hpp"


#if defined(CHAOS_REGION_pal)
#define kTicksPerSecond 50
#elif defined(CHAOS_REGION_usa)
#define kTicksPerSecond 60
#else
// invalid/unported, assume 60
#define kTicksPerSecond 60
#endif

u32 chaosSecToTicks(float secondTime) {
	float tickCount = secondTime * (kTicksPerSecond * 1000);
	return static_cast<u32>(tickCount);
}

float chaosTickToSecs(u32 tickCount) {
	return static_cast<float>(tickCount) / (static_cast<float>(kTicksPerSecond) * 1000.f);
}
