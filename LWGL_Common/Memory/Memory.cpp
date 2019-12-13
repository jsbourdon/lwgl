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
