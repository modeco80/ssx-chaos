#include <ml/mem.h>

void* mlCalloc(u32 nmemb, u32 membsz) {
    void* p = mlMalloc(nmemb * membsz);
    if(!p)
        return vnil;
    mlMemSet(p, 0, nmemb * membsz);
    return p;
}
