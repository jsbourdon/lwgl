#pragma once

#include <cstdint>

namespace lwgl
{
    namespace memory
    {
        class RingBuffer
        {
        public:

            RingBuffer(size_t byteSize);
            ~RingBuffer();

            template<typename T>
            void* Allocate();

            void* Allocate(size_t byteSize, size_t alignment = sizeof(uint32_t));
            void EndFrame();

            uintptr_t m_RingStart;
            uintptr_t m_RingEnd;
            uintptr_t m_NextAddr;
        #ifdef _DEBUG
            uintptr_t m_FrameStart;
        #endif
        };

        template<typename T>
        void* RingBuffer::Allocate()
        {
            return Allocate(sizeof(T), alignof(T));
        }
    }
}