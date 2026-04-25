#ifndef REAL_REALMEM_H
#define REAL_REALMEM_H

// Minimal interface to realmem library, feel free to expand

#ifdef __cplusplus
extern "C" {
#endif

enum {
	// default allocation
	MB_LOW = 0
};

void* MEM_alloc(const char* name, u32 size, i32 flags);
void MEM_free(void* pv);

#ifdef __cplusplus
}
#endif

#endif
