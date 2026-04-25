#include <ml/abort.h>
#include <ml/mem.h>
#include <ml/string.h>

// libc symbols
extern "C" u32 strlen(const char*);
extern "C" void* memset(void* dst, i32 c, usize len);
extern "C" void* memcpy(void* dst, const void* src, u32 len);

u32 mlStrlen(const char* pszString) {
	mlASSERT(pszString != nil(const char*));
	return strlen(pszString);
}

void mlMemSet(void* pBuffer, int c, u32 len) {
	memset(pBuffer, c, len);
}

void mlMemCpy(void* pDestBuffer, const void* pSrcBuffer, u32 copyLen) {
	memcpy(pDestBuffer, pSrcBuffer, copyLen);
}
