#pragma once

#include <malloc.h>
#include <new>

// Global allocation operator overrides
void* operator new(std::size_t count);
void* operator new[](std::size_t count);
void operator delete(void *ptr);
void operator delete[](void *ptr);

#ifdef __cpp_aligned_new
    void* operator new(std::size_t count, std::align_val_t al);
    void* operator new[](std::size_t count, std::align_val_t al);
    void operator delete(void *ptr, std::align_val_t al);
    void operator delete[](void *ptr, std::align_val_t al);
#endif


namespace lwgl
{
    namespace memory
    {
        void ForceGlobalAllocations(bool force);

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

        inline size_t Align(size_t size, size_t alignment)
        {
            size_t factor = alignment - 1;
            return (size + factor) & ~factor;
        }
    }
}