#pragma once

#include <pch.h>

namespace lwgl
{
    namespace data
    {
        class IndexFreeList
        {
        public:
            
            IndexFreeList() = default;
            ~IndexFreeList();

            void Init(uint32_t capacity);
            Handle Add(uint32_t index);
            uint32_t Remove(const Handle &handle);
            uint32_t Get(const Handle &handle) const;
            void Update(const Handle &handle, uint32_t index);
            uint32_t Count() const { return m_Count; }

        private:

            struct Item
            {
                Handle m_Handle;
                uint32_t m_Next;
            };

            bool ValidateHandle(const Handle& handle) const;

            Item* m_Items { nullptr };
            uint32_t m_Capacity;
            uint32_t m_Count { 0 };
            uint32_t m_FreeListIndex { 0 };
        };
    }
}