#pragma once

#include <cstdint>
#include "IndexFreeList.h"
#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace data
    {
        // Data structure used to keep a mutable list of objects always 
        // tightly packed so it is possible to iterate over them linearly.
        // As long as nothing is removed from the list, indices are stable.
        // When objects are removed, indices are not longer valid.
        // 
        // To keep a stable access to objects in the array, `Handle` are 
        // used.
        //
        // All operations in this list are in O(1)
        //
        template<typename T>
        class PackedArray
        {
        public:

            PackedArray(uint32_t capacity);
            PackedArray(PackedArray&& other) noexcept;
            PackedArray& operator=(PackedArray &&other) noexcept;
            ~PackedArray();

            Handle Add(T &value);
            void Remove(const Handle &handle);
            T* Get(const Handle &handle) const;
            T* GetIterator() { return m_Values; }
            const T* GetIterator() const { return m_Values; }
            uint32_t Count() const { return m_Lookup.Count(); }
            Handle GetHandleFromIndex(uint32_t index) const;

        private:

            IndexFreeList   m_Lookup {};
            Handle*         m_LookupHandles { nullptr };
            T*              m_Values { nullptr };
            uint32_t        m_NextIndex { 0 };
        };
    }
}

using namespace lwgl;
using namespace lwgl::data;

template<typename T>
PackedArray<T>::PackedArray(uint32_t capacity)
{
    m_Lookup.Init(capacity);
    m_LookupHandles = static_cast<Handle*>(lwgl::memory::AlignedAlloc(capacity, alignof(Handle)));
    m_Values = static_cast<T*>(lwgl::memory::AlignedAlloc(capacity, alignof(T)));
}

template<typename T>
PackedArray<T>::PackedArray(PackedArray&& other) noexcept
{
    m_Lookup = std::move(other.m_Lookup);
    m_LookupHandles = other.m_LookupHandles;
    m_Values = other.m_Values;
    m_NextIndex = other.m_NextIndex;

    other.m_LookupHandles = nullptr;
    other.m_Values = nullptr;
}

template<typename T>
PackedArray<T>& PackedArray<T>::operator=(PackedArray<T> &&other) noexcept
{
    m_Lookup = std::move(other.m_Lookup);
    m_LookupHandles = other.m_LookupHandles;
    m_Values = other.m_Values;
    m_NextIndex = other.m_NextIndex;

    other.m_LookupHandles = nullptr;
    other.m_Values = nullptr;

    return *this;
}

template<typename T>
PackedArray<T>::~PackedArray()
{
    lwgl::memory::FreeAlignedAlloc(m_Values);
    lwgl::memory::FreeAlignedAlloc(m_LookupHandles);
}

template<typename T>
lwgl::Handle PackedArray<T>::Add(T &value)
{
    assert(m_NextIndex < m_Lookup.Capacity());

    uint32_t newIndex = m_NextIndex++;
    Handle newHdl = m_Lookup.Add(newIndex);
    m_LookupHandles[newIndex] = newHdl;
    m_Values[newIndex] = value;

    return newHdl;
}

template<typename T>
void PackedArray<T>::Remove(const Handle &handle)
{
    assert(m_NextIndex > 0);

    --m_NextIndex;
    uint32_t valueIndex = m_Lookup.Remove(handle);

    m_Values[valueIndex] = m_Values[m_NextIndex];
    m_LookupHandles[valueIndex] = m_LookupHandles[m_NextIndex];

    m_Lookup.Update(m_LookupHandles[valueIndex], valueIndex);
}

template<typename T>
T* PackedArray<T>::Get(const Handle &handle) const
{
    uint32_t index = m_Lookup.Get(handle);
    return &m_Values[index];
}

template<typename T>
lwgl::Handle PackedArray<T>::GetHandleFromIndex(uint32_t index) const
{
    return m_LookupHandles[index];
}
