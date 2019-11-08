#include <pch.h>
#include "IndexFreeList.h"
#include <assert.h>

using namespace lwgl;
using namespace lwgl::data;

IndexFreeList::IndexFreeList(IndexFreeList&& other) noexcept
{
    m_Items = other.m_Items;
    m_Capacity = other.m_Capacity;
    m_Count = other.m_Count;
    m_FreeListIndex = other.m_FreeListIndex;

    other.m_Items = nullptr;
}

IndexFreeList& IndexFreeList::operator=(IndexFreeList &&other) noexcept
{
    m_Items = other.m_Items;
    m_Capacity = other.m_Capacity;
    m_Count = other.m_Count;
    m_FreeListIndex = other.m_FreeListIndex;

    other.m_Items = nullptr;

    return *this;
}

void IndexFreeList::Init(uint32_t capacity)
{
    m_Capacity = capacity;
    m_Items = static_cast<Item*>(lwgl::core::AlignedAlloc(capacity, alignof(Item)));

    for (uint32_t i = 0; i < m_Capacity; ++i)
    {
        m_Items[i].m_Next = i + 1;
    }
}

IndexFreeList::~IndexFreeList()
{
    if (m_Items)
    {
        lwgl::core::FreeAlignedAlloc(m_Items);
    }
}

Handle IndexFreeList::Add(uint32_t index)
{
    assert((m_FreeListIndex < m_Capacity && m_Count < m_Capacity) && "Free list is full");

    ++m_Count;

    Handle handle { m_FreeListIndex };
    Item &item = m_Items[m_FreeListIndex];
    m_FreeListIndex = item.m_Next;
    item.m_Handle.m_Value = index;

#if VALIDATE_HANDLES
    handle.m_Generation = item.m_Handle.m_Generation;
#endif

    return handle;
}

uint32_t IndexFreeList::Remove(const Handle &handle)
{
    assert(ValidateHandle(handle));

    --m_Count;

    Item &item = m_Items[handle.m_Value];
    item.m_Next = m_FreeListIndex;
    m_FreeListIndex = handle.m_Value;

#if VALIDATE_HANDLES
    ++item.m_Handle.m_Generation; // Bump generation so any remaining handle copy is invalidated
#endif

    return item.m_Handle.m_Value;
}

uint32_t IndexFreeList::Get(const Handle &handle) const
{
    assert(ValidateHandle(handle));
    Item &item = m_Items[handle.m_Value];
    return item.m_Handle.m_Value;
}

void IndexFreeList::Update(const Handle &handle, uint32_t index)
{
    assert(ValidateHandle(handle));
    Item &item = m_Items[handle.m_Value];
    item.m_Handle.m_Value = index;
}

bool IndexFreeList::ValidateHandle(const Handle& handle) const
{
    return (handle.m_Value < m_Capacity) 
#if VALIDATE_HANDLES
        && (handle.m_Generation == m_Items[handle.m_Value].m_Handle.m_Generation)
#endif
        ;
}
