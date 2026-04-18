#include "../ml/hw/eeuart.h"

// Since we're not using the C main entry point, we have to call these by hand
extern "C" void __do_global_ctors();
extern "C" void __do_global_dtors();

extern "C" void chaosMain() {
    __do_global_ctors();

    eeUartPuts("Hello World?");
}
