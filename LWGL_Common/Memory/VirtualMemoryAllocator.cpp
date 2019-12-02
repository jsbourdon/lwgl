#include <pch.h>
#include <algorithm>
#include "VirtualMemoryAllocator.h"
#include "LWGL_Common/SystemInfo/SystemInfo.h"

using namespace lwgl::memory;
using namespace lwgl::system;

VirtualMemoryAllocator VirtualMemoryAllocator::s_Allocator;

VirtualMemoryAllocator::~VirtualMemoryAllocator()
{
    VMHeader *pHeader = m_FreeHead;
    while (pHeader)
    {
        VMHeader *pNext = pHeader->m_Next;
        ForceReleaseToOS(pHeader);
        pHeader = pNext;
    }

    m_FreeHead = m_FreeTail = nullptr;
}

VirtualMemoryAllocator::VirtualMemoryAllocator()
{
    SystemInfo sysInfo = lwgl::system::GetSystemInfo();
    m_AllocationPageSize = sysInfo.m_AllocationPageSize;
}

void* VirtualMemoryAllocator::Allocate(size_t byteSize, size_t commitByteSize, size_t alignment)
{
    LWGL_ASSERT(alignment <= s_Allocator.m_AllocationPageSize, "Unsupported alignment");

    std::unique_lock<std::mutex> lock(s_Allocator.m_Mutex);

    alignment = std::max(alignof(VMHeader), alignment);
    size_t requiredOverhead = Align(sizeof(VMHeader), alignment);
    size_t alignedByteSize = Align(byteSize + requiredOverhead, s_Allocator.m_AllocationPageSize);
    size_t alignedCommitByteSize = Align(commitByteSize + requiredOverhead, s_Allocator.m_AllocationPageSize);

    void *pMemory = nullptr;
    VMHeader *pHeader = FindReleasedMemory(alignedByteSize);

    if (pHeader)
    {
        size_t allocationStartOffset = Align(sizeof(VMHeader), pHeader->m_Alignment) - sizeof(VMHeader);
        pMemory = reinterpret_cast<uint8_t*>(pHeader) - allocationStartOffset;
    }
    else
    {
        pMemory = NativeReserve(alignedByteSize);
    }

    NativeCommit(pMemory, alignedCommitByteSize);

    VMHeader *pReturnedAddress = reinterpret_cast<VMHeader*>(reinterpret_cast<uint8_t*>(pMemory) + requiredOverhead);

    VMHeader &header = *(pReturnedAddress - 1);
    header.m_Next = nullptr;
    header.m_Size = alignedByteSize;
    header.m_Alignment = alignment;
#ifdef _DEBUG
    header.m_Marker = MARKER;
#endif

    return pReturnedAddress;
}

void VirtualMemoryAllocator::Release(void *pMemory)
{
    std::unique_lock<std::mutex> lock(s_Allocator.m_Mutex);
    
    VMHeader *pHeader = (reinterpret_cast<VMHeader*>(pMemory) - 1);
    LWGL_ASSERT(pHeader->m_Marker == MARKER, "Invalid address sent to VirtualMemoryAllocator::Release");
    LWGL_ASSERT(pHeader->m_Next == nullptr, "Releasing already released memory");

    if (s_Allocator.m_FreeTail)
    {
        s_Allocator.m_FreeTail->m_Next = pHeader;
        s_Allocator.m_FreeTail = pHeader;
    }
    else
    {
        s_Allocator.m_FreeHead = s_Allocator.m_FreeTail = pHeader;
    }

    // Decommit everything but the minimum required to keep the header alive
    size_t allocationStartOffset = Align(sizeof(VMHeader), pHeader->m_Alignment);
    size_t startAddr = reinterpret_cast<size_t>(reinterpret_cast<uint8_t*>(pMemory) - allocationStartOffset);
    size_t decommitAddr = Align(reinterpret_cast<size_t>(pMemory), s_Allocator.m_AllocationPageSize);
    size_t pinnedByteSize = decommitAddr - startAddr;
    NativeDecommit(reinterpret_cast<void*>(decommitAddr), pHeader->m_Size - pinnedByteSize);
}

void VirtualMemoryAllocator::ForceReleaseToOS(void *pMemory)
{
    std::unique_lock<std::mutex> lock(s_Allocator.m_Mutex);

    VMHeader header = *(reinterpret_cast<VMHeader*>(pMemory) - 1);
    size_t allocationStartOffset = Align(sizeof(VMHeader), header.m_Alignment);
    void *pAllocationAddress = reinterpret_cast<uint8_t*>(pMemory) - allocationStartOffset;

    LWGL_ASSERT(header.m_Marker == MARKER, "Invalid address sent to VirtualMemoryAllocator::Release");
    LWGL_ASSERT(header.m_Next == nullptr, "Releasing already released memory");

    NativeRelease(pAllocationAddress);
}

void VirtualMemoryAllocator::Commit(void *pMemory, size_t size)
{
    std::unique_lock<std::mutex> lock(s_Allocator.m_Mutex);
    NativeCommit(pMemory, size);
}

void VirtualMemoryAllocator::Decommit(void *pMemory, size_t size)
{
    std::unique_lock<std::mutex> lock(s_Allocator.m_Mutex);
    NativeDecommit(pMemory, size);
}

void VirtualMemoryAllocator::ForceReleaseToOS(VMHeader *pHeader)
{
    LWGL_ASSERT(pHeader->m_Marker == MARKER, "Invalid address sent to VirtualMemoryAllocator::Release");

    uint8_t *pMemory = reinterpret_cast<uint8_t*>(pHeader + 1);
    size_t allocationStartOffset = Align(sizeof(VMHeader), pHeader->m_Alignment);
    void *pAllocationAddress = pMemory - allocationStartOffset;

    NativeRelease(pAllocationAddress);
}

VirtualMemoryAllocator::VMHeader* VirtualMemoryAllocator::FindReleasedMemory(size_t byteSize)
{
    VMHeader *pPreviousHeader = nullptr;
    VMHeader *pHeader = s_Allocator.m_FreeHead;

    if (pHeader)
    {
        if (pHeader->m_Size >= byteSize)
        {
            s_Allocator.m_FreeHead = pHeader->m_Next;
        }
        else
        {
            pPreviousHeader = pHeader;
            pHeader = pHeader->m_Next;

            while (pHeader)
            {
                if (pHeader->m_Size >= byteSize)
                {
                    pPreviousHeader->m_Next = pHeader->m_Next;
                    pHeader->m_Next = nullptr;
                    break;
                }
                else
                {
                    pPreviousHeader = pHeader;
                    pHeader = pHeader->m_Next;
                }
            }
        }

        if (s_Allocator.m_FreeTail == pHeader)
        {
            s_Allocator.m_FreeTail = pPreviousHeader;
        }
    }
    
    return pHeader;
}

#include INCLUDE_IMPLEMENTATION(VirtualMemoryAllocator)
