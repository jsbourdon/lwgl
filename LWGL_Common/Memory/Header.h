#pragma once

#include "Memory.h"

namespace lwgl
{
    namespace memory
    {
        typedef size_t MemoryBookmark;

        class FrameMemoryAllocator
        {
        public:

            FrameMemoryAllocator() {};
            FrameMemoryAllocator(size_t poolByteSize);
            ~FrameMemoryAllocator();

            void Init(size_t poolByteSize);

            void* Allocate(size_t byteSize, size_t alignment = sizeof(uintptr_t));
            MemoryBookmark SetBookmark() const { return m_NextByte; }
            void Reset(MemoryBookmark bookmark);
            void Reset();
            size_t GetAllocatedByteCount() const { return m_NextByte; }

        private:

            static const size_t s_DefaultAlignment = 16;

            size_t Align(size_t size, size_t alignment);

            uint8_t* m_MemoryPool { nullptr };
            size_t m_PoolByteSize { 0 };
            size_t m_NextByte { 0 };
        };
    }
}
