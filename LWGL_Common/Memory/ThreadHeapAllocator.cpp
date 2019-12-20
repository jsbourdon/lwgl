#include <pch.h>
#include "ThreadHeapAllocator.h"
#include "VirtualMemoryAllocator.h"

using namespace lwgl::memory;

thread_local ThreadHeapAllocator ThreadHeapAllocator::s_Allocator;

ThreadHeapAllocator::~ThreadHeapAllocator()
{
    if (m_MemoryPool != 0)
    {
        VirtualMemoryAllocator::Release(m_pFreeRanges);
        VirtualMemoryAllocator::Release(reinterpret_cast<void*>(m_MemoryPool));
    }
}

void ThreadHeapAllocator::Init(size_t heapByteSize, size_t initialCommitByteSize)
{
    uint32_t pageSize = VirtualMemoryAllocator::GetSystemPageSize();
    uint32_t pageCount = static_cast<uint32_t>(Align(heapByteSize, pageSize)) / pageSize;
    uint32_t committedPageCount = static_cast<uint32_t>(Align(initialCommitByteSize, pageSize)) / pageSize;
    size_t metadataOverhead = (sizeof(uint32_t) + sizeof(FreeRange)) * pageCount;

    s_Allocator.m_MemoryPool = reinterpret_cast<uintptr_t>(VirtualMemoryAllocator::Allocate(heapByteSize, initialCommitByteSize, pageSize));
    s_Allocator.m_pFreeRanges = static_cast<FreeRange*>(VirtualMemoryAllocator::Allocate(metadataOverhead, metadataOverhead));
    s_Allocator.m_pPages = reinterpret_cast<uint32_t*>(s_Allocator.m_pFreeRanges + pageCount);
    
    s_Allocator.m_SystemPageSize = pageSize;
    s_Allocator.m_TotalPageCount = pageCount;
    s_Allocator.m_CommittedPageCount = committedPageCount;
    s_Allocator.m_NextPageIndex = 0;
}

void* ThreadHeapAllocator::Allocate(size_t byteSize)
{
    return s_Allocator.AllocateInternal(byteSize);
}

void ThreadHeapAllocator::Release(void *pMemory)
{
    s_Allocator.ReleaseInternal(pMemory);
}

size_t ThreadHeapAllocator::AllocationSize(void *pMemory)
{
    return s_Allocator.AllocationSizeInternal(pMemory);
}

void ThreadHeapAllocator::Defrag()
{
    s_Allocator.DefragInternal();
}

void* ThreadHeapAllocator::AllocateInternal(size_t byteSize)
{
    void *pAllocation = nullptr;

    uint32_t requiredPageCount = static_cast<uint32_t>(Align(byteSize, m_SystemPageSize)) / m_SystemPageSize;
    uint32_t newNextPageIndex = m_NextPageIndex + requiredPageCount;
    LWGL_ASSERT(requiredPageCount <= m_TotalPageCount, "ThreadHeapAllocator: Requested allocation exceeds allocator total byte size.");
    
    if (newNextPageIndex < m_CommittedPageCount)
    {
        m_pPages[m_NextPageIndex] = requiredPageCount;
        pAllocation = PageIndexToAddress(m_NextPageIndex);
        m_NextPageIndex = newNextPageIndex;
    }
    else
    {
        uint32_t freePagesStartIndex = SearchFreePages(requiredPageCount);
        if (freePagesStartIndex != UINT32_MAX)
        {
            m_pPages[freePagesStartIndex] = requiredPageCount;
            pAllocation = PageIndexToAddress(freePagesStartIndex);
        }
        else
        {
            uint32_t newRequiredCommitCount = newNextPageIndex - m_CommittedPageCount;
            uint32_t remainingReservedPagesCount = m_TotalPageCount - m_CommittedPageCount;
            if (newRequiredCommitCount <= remainingReservedPagesCount)
            {
                CommitMorePages(newRequiredCommitCount);

                m_pPages[m_NextPageIndex] = requiredPageCount;
                pAllocation = PageIndexToAddress(m_NextPageIndex);
                m_NextPageIndex = newNextPageIndex;
            }
        }
    }

    LWGL_ASSERT(pAllocation, "ThreadHeapAllocator: Out of Memory");

    return pAllocation;
}

void ThreadHeapAllocator::ReleaseInternal(void *pMemory)
{
    LWGL_ASSERT(
        (reinterpret_cast<uintptr_t>(pMemory) >= m_MemoryPool) && 
        (reinterpret_cast<uintptr_t>(pMemory) < (m_MemoryPool + m_TotalPageCount * m_SystemPageSize)),
        "ThreadHeapAllocator::ReleaseInternal: given address was not allocated from this allocator.");

    uint32_t pageIndex = AddressToPageIndex(pMemory);
    FreeRange &range = m_pFreeRanges[m_FreeRangesCount++];
    range.m_Index = pageIndex;
    range.m_Count = m_pPages[pageIndex];
}

size_t ThreadHeapAllocator::AllocationSizeInternal(void *pMemory)
{
    uint32_t pageIndex = AddressToPageIndex(pMemory);
    return m_pPages[pageIndex] * m_SystemPageSize;
}

void ThreadHeapAllocator::DefragInternal()
{
    // #todo Implement allocator memory defrag
}

void ThreadHeapAllocator::CommitMorePages(uint32_t pageCount)
{
    uint32_t reservedPageCount = m_TotalPageCount - m_CommittedPageCount;
    LWGL_ASSERT(pageCount <= reservedPageCount, 
        "ThreadHeapAllocator::CommitMorePages: Cannot commit requested number of pages. Not enough reserved memory");

    size_t newCommittedPageCount = std::min(std::max(pageCount, m_CommittedPageCount * 2), reservedPageCount);
    VirtualMemoryAllocator::Commit(PageIndexToAddress(m_CommittedPageCount), newCommittedPageCount * m_SystemPageSize);
}

uint32_t ThreadHeapAllocator::SearchFreePages(uint32_t pageCount)
{
    uint32_t rangeStartIndex = UINT32_MAX;

    const uint32_t rangeCount = m_FreeRangesCount;
    for (uint32_t i = 0; i < rangeCount; ++i)
    {
        FreeRange &range = m_pFreeRanges[i];
        if (range.m_Count >= pageCount)
        {
            rangeStartIndex = range.m_Index;
            m_pFreeRanges[i] = m_pFreeRanges[--m_FreeRangesCount];
            break;
        }
    }

    return rangeStartIndex;
}

void* ThreadHeapAllocator::PageIndexToAddress(uint32_t index)
{
    uintptr_t offset = uintptr_t(index) * uintptr_t(m_SystemPageSize);
    return reinterpret_cast<void*>(m_MemoryPool + offset);
}

uint32_t ThreadHeapAllocator::AddressToPageIndex(void *pAddress)
{
    uintptr_t address = reinterpret_cast<uintptr_t>(pAddress);
    uintptr_t offset = address - m_MemoryPool;

    return static_cast<uint32_t>(offset / m_SystemPageSize);
}
