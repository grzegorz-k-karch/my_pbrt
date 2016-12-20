#include "memory.h"
#include "port.h"
#include <malloc.h>

namespace pbrt {

void *AllocAligned(size_t size) {

    return memalign(PBRT_L1_CACHE_LINE_SIZE, size);
}

void FreeAligned(void* ptr) {

    if (!ptr) {
        return;
    }
    free(ptr);
}


}//namespace pbrt
