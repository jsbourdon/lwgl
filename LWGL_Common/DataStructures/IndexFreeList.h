#pragma once

#include <cstdint>
#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace data
    {
        // Free-list of indices using `Handle` to access.
        // Used in a `PackedArray` to keep the link between
        // handles and actual array indices.
        //
        // All operations in this free-list are in O(1)
        //
        class IndexFreeList
        {
        public:

            IndexFreeList() = default;
            IndexFreeList(IndexFreeList&& other) noexcept;
            IndexFreeList& operator=(IndexFreeList &&other) noexcept;
            ~IndexFreeList();

            void Init(uint32_t capacity);
            Handle Add(uint32_t index);
            uint32_t Remove(const Handle &handle);
            uint32_t Get(const Handle &handle) const;
            void Update(const Handle &handle, uint32_t index);
            uint32_t Count() const { return m_Count; }
            uint32_t Capacity() const { return m_Capacity; }

        private:

            struct Item
            {
                Handle m_Handle;
                uint32_t m_Next;
            };

            bool ValidateHandle(const Handle& handle) const;

            Item*       m_Items { nullptr };
            uint32_t    m_Capacity;
            uint32_t    m_Count { 0 };
            uint32_t    m_FreeListIndex { 0 };
        };
    }
}
