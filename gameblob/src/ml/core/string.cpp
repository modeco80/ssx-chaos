#include <ml/string.h>
#include <ml/mem.h>
#include <ml/abort.h>

u32 mlStrLen(const char* pszString) {
    mlASSERT(pszString);

    const char* start = pszString;
    while(*pszString)
            pszString++;
    return (pszString - start);
}

char* mlStrDup(const char* pszString) {
    mlASSERT(pszString);
    if(!pszString)
        return nil(char*);

    u32 stringLength = mlStrLen(pszString);
    char* pDuplicatedString = (char*)mlCalloc(stringLength + 1, sizeof(char));

    mlASSERT(pDuplicatedString != nil(char*));
    if(!pDuplicatedString)
        return nil(char*);

    mlMemCpy(&pDuplicatedString[0], &pszString[0], stringLength);
    return pDuplicatedString;
}
