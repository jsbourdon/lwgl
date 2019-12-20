#include <pch.h>
#include "Memory.h"
#include "ThreadHeapAllocator.h"
#include "VirtualMemoryAllocator.h"

using namespace lwgl::memory;

static bool s_ForceGlobalAlloc = false;

void lwgl::memory::ForceGlobalAllocations(bool force)
{
    s_ForceGlobalAlloc = force;
}

void* lwgl::memory::AlignedAlloc(size_t size, size_t /*alignment*/)
{
    // Aligned on page size by default
    return ThreadHeapAllocator::Allocate(size);
}

void* lwgl::memory::AlignedRealloc(void *pMemory, size_t newSize, size_t /*alignment*/)
{
    // Aligned on page size by default
    void *pNewAddr = ThreadHeapAllocator::Allocate(newSize);

    size_t originalSize = ThreadHeapAllocator::AllocationSize(pMemory);
    size_t copySize = std::min(originalSize, newSize);

    memcpy(pNewAddr, pMemory, copySize);
    ThreadHeapAllocator::Release(pMemory);

    return pNewAddr;
}

void lwgl::memory::FreeAlignedAlloc(void *pAlloc)
{
    ThreadHeapAllocator::Release(pAlloc);
}

void* operator new(std::size_t count)
{
    if (s_ForceGlobalAlloc)
    {
        return VirtualMemoryAllocator::Allocate(count, count);
    }
    
    return ThreadHeapAllocator::Allocate(count);
}

void* operator new[](std::size_t count)
{
    if (s_ForceGlobalAlloc)
    {
        return VirtualMemoryAllocator::Allocate(count, count);
    }

    return ThreadHeapAllocator::Allocate(count);
}

void operator delete(void *ptr)
{
    if (s_ForceGlobalAlloc)
    {
        VirtualMemoryAllocator::Release(ptr);
    }
    else
    {
        ThreadHeapAllocator::Release(ptr);
    }
}

void operator delete[](void *ptr)
{
    if (s_ForceGlobalAlloc)
    {
        VirtualMemoryAllocator::Release(ptr);
    }
    else
    {
        ThreadHeapAllocator::Release(ptr);
    }
}

#ifdef __cpp_aligned_new

void* operator new(std::size_t count, std::align_val_t al)
{
    if (s_ForceGlobalAlloc)
    {
        return VirtualMemoryAllocator::Allocate(count, count, static_cast<size_t>(al));
    }

    // The allocator already aligns on system page size
    return ThreadHeapAllocator::Allocate(count);
}


void* operator new[](std::size_t count, std::align_val_t al)
{
    if (s_ForceGlobalAlloc)
    {
        return VirtualMemoryAllocator::Allocate(count, count, static_cast<size_t>(al));
    }

    // The allocator already aligns on system page size
    return ThreadHeapAllocator::Allocate(count);
}

void operator delete(void *ptr, std::align_val_t al)
{
    if (s_ForceGlobalAlloc)
    {
        VirtualMemoryAllocator::Release(ptr);
    }
    else
    {
        ThreadHeapAllocator::Release(ptr);
    }
}

void operator delete[](void *ptr, std::align_val_t al)
{
    if (s_ForceGlobalAlloc)
    {
        VirtualMemoryAllocator::Release(ptr);
    }
    else
    {
        ThreadHeapAllocator::Release(ptr);
    }
}

#endif // __cpp_aligned_new
