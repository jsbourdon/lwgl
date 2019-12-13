#pragma once

#include <stdint.h>
#include "LWGL_Common/Memory/Memory.h"

namespace lwgl
{
    namespace memory
    {
        class ThreadHeapAllocator
        {
        public:

            ThreadHeapAllocator() = default;
            ~ThreadHeapAllocator();

            static void     Init(size_t heapByteSize, size_t initialCommitByteSize);
            static void*    Allocate(size_t byteSize);
            static void     Release(void *pMemory);
            static void     Defrag();

        private:

            struct FreeRange
            {
                uint32_t m_Index;
                uint32_t m_Count;
            };

            void*       AllocateInternal(size_t byteSize);
            void        ReleaseInternal(void *pMemory);
            void        DefragInternal();

            void        CommitMorePages(uint32_t pageCount);
            uint32_t    SearchFreePages(uint32_t pageCount);
            void*       PageIndexToAddress(uint32_t index);
            uint32_t    AddressToPageIndex(void *pAddress);

            thread_local static ThreadHeapAllocator s_Allocator;

            uintptr_t   m_MemoryPool { 0 };
            FreeRange*  m_pFreeRanges { nullptr };
            uint32_t*   m_pPages { nullptr };
            uint32_t    m_SystemPageSize { 0 };
            uint32_t    m_TotalPageCount { 0 };
            uint32_t    m_CommittedPageCount { 0 };
            uint32_t    m_NextPageIndex { 0 };
            uint32_t    m_FreeRangesCount { 0 };
        };
    }
}