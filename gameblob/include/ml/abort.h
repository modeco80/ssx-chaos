#ifndef ML_API_ABORT_H
#define ML_API_ABORT_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    /// Aborts.
    ml_noreturn void mlAbort(const char* pszMsg);

#define ___assert_stringify(xx) #xx
#define __assert_stringify(x) ___assert_stringify(x)

    /// Assertion funtimes
#define mlASSERT(xpr) \
    if(!(xpr)) { mlAbort("Assertion \"" __assert_stringify(xpr) "\" failed in " __FILE__ ":" __assert_stringify(__LINE__)); }


#ifdef __cplusplus
}
#endif

#endif
