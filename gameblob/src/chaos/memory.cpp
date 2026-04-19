// REALMEM memory callbacks implementation
#include <ml/mem.h>
#include <real/realmem.h>

void* mlMalloc(u32 size) {
	return MEM_alloc("CHAOS", size, MB_LOW);
}

void mlFree(void* pv) {
	if(pv)
		MEM_free(pv);
}

void mlMemSet(void* pvBuffer, i32 c, u32 len) {
	MEM_fill(pvBuffer, c, len);
}
