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
        void    ForceGlobalAllocations(bool force);

        void*   AlignedAlloc(size_t size, size_t alignment);
        void*   AlignedRealloc(void *pMemory, size_t newSize, size_t alignment);
        void    FreeAlignedAlloc(void *pAlloc);

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