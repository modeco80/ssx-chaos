#ifndef ML_API_STRING_H
#define ML_API_STRING_H

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    u32 mlStrLen(const char* str);
    i32 mlStrCmp(const char* pszLeft, const char* pszRight);

    /// Duplicates a string, allocating it. The memory must be freed with
    /// the [mlFree()] function once the duplicated string is no longer needed.
    char* mlStrDup(const char* pszString);

#ifdef __cplusplus
}
#endif

#endif
