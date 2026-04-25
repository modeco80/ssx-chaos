#include "../../ml/hw/eeuart.h"

extern "C" ml_noreturn void mlAbort(const char* msg) {
	eeUartPuts("mlAbort(): ");
	eeUartPuts(msg);
	// Spin forever to uphold never returning
	while(1) {
	}
}
