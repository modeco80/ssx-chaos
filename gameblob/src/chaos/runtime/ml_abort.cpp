#include "../utils/log.hpp"
#include <ml/types.h>

extern "C" ml_noreturn void mlAbort(const char* msg) {
	chaosLogf(LogErr, "mlAbort(): %s", msg);
	// maybe dump stack?
	// Spin forever to uphold never returning
	while(1) {
	}
}
