#pragma once

#include <malloc.h>

namespace lwgl
{
    namespace memory
    {
        // MSFT: aligned_alloc() will probably never be implemented, as C11 specified it in a way that’s incompatible 
        // with our implementation(namely, that free() must be able to handle highly aligned allocations).
        inline void* AlignedAlloc(size_t size, size_t alignment)
        {
            return _aligned_malloc(size, alignment);
        }

        inline void* AlignedRealloc(void *mem, size_t newSize, size_t alignment)
        {
            return _aligned_realloc(mem, newSize, alignment);
        }

        inline void FreeAlignedAlloc(void *pAlloc)
        {
            _aligned_free(pAlloc);
        }

        inline void* StackAlloc(size_t size)
        {
            return _malloca(size);
        }
    }
}