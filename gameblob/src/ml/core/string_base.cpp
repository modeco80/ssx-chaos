#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/string.h>

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
