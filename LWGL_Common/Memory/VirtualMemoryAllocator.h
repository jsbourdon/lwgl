#pragma once

#include <mutex>

namespace lwgl
{
    namespace memory
    {
        class VirtualMemoryAllocator
        {
        public:

            static void* Allocate(size_t byteSize, size_t commitByteSize, size_t alignment = sizeof(uintptr_t));
            static void Commit(void *pMemory, size_t size);
            static void Decommit(void *pMemory, size_t size);
            static void Release(void *pMemory);
            static void ForceReleaseToOS(void *pMemory);

        private:

            static constexpr size_t MARKER = 0xDEADBEEFBADDEED1;

            struct alignas(sizeof(size_t)) VMHeader
            {
                VMHeader* m_Next { nullptr };
                size_t m_Size { 0 };
                size_t m_Alignment { 0 };
            #ifdef _DEBUG
                size_t m_Marker { MARKER };
            #endif
            };

            static VMHeader* FindReleasedMemory(size_t byteSize);
            static void ForceReleaseToOS(VMHeader *pHeader);

            static void* NativeReserve(size_t byteSize);
            static void NativeCommit(void *pMemory, size_t size);
            static void NativeDecommit(void *pMemory, size_t size);
            static void NativeRelease(void *pMemory);

            static VirtualMemoryAllocator s_Allocator;

            VirtualMemoryAllocator();
            ~VirtualMemoryAllocator();

            VMHeader* m_FreeHead { nullptr };
            VMHeader* m_FreeTail { nullptr };
            std::mutex m_Mutex {};
            uint32_t m_AllocationPageSize { 0 };
        };
    }
}
