#pragma once

#include "LWGL_Common/Memory/VirtualMemoryAllocator.h"

using namespace lwgl::memory;

void* VirtualMemoryAllocator::NativeReserve(size_t byteSize)
{
    return VirtualAlloc(NULL, byteSize, MEM_RESERVE, PAGE_READWRITE);
}

void VirtualMemoryAllocator::NativeCommit(void *pMemory, size_t size)
{
    VirtualAlloc(pMemory, size, MEM_COMMIT, PAGE_READWRITE);
}

void VirtualMemoryAllocator::NativeDecommit(void *pMemory, size_t size)
{
    VirtualFree(pMemory, size, MEM_DECOMMIT);
}

void VirtualMemoryAllocator::NativeRelease(void *pMemory)
{
    VirtualFree(pMemory, 0, MEM_RELEASE);
}

