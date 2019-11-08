#pragma once

#include <pch.h>
#include "IndexFreeList.h"

namespace lwgl
{
    namespace data
    {
        template<typename T>
        class PackedArray
        {
        public:

            PackedArray(uint32_t capacity);
            ~PackedArray();

            Handle Add(T &value);
            Handle Emplace(T *&out_memory);
            void Remove(const Handle &handle);
            T* Get(const Handle &handle) const;
            T* GetIterator();

        private:

            IndexFreeList   m_Lookup {};
            Handle*         m_LookupHandles {};
            T*              m_Values { nullptr };
            uint32_t        m_NextIndex { 0 };
        };
    }
}

template<typename T>
lwgl::data::PackedArray<T>::PackedArray(uint32_t capacity)
{
    m_Lookup.Init(capacity);
    m_LookupHandles = static_cast<Handle*>(lwgl::core::AlignedAlloc(capacity, alignof(Handle)));
    m_Values = static_cast<T*>(lwgl::core::AlignedAlloc(capacity, alignof(T)));
}

template<typename T>
lwgl::data::PackedArray<T>::~PackedArray()
{
    lwgl::core::FreeAlignedAlloc(m_Values);
    lwgl::core::FreeAlignedAlloc(m_LookupHandles);
}

template<typename T>
lwgl::Handle lwgl::data::PackedArray<T>::Add(T &value)
{
    uint32_t newIndex = m_NextIndex++;
    lwgl::Handle newHdl = m_Lookup.Add(newIndex);
    T &newItem = m_Values[newIndex];
    m_LookupHandles[newIndex] = newHdl;
    newItem.m_Value = value;

    return newHdl;
}

template<typename T>
lwgl::Handle lwgl::data::PackedArray<T>::Emplace(T *&out_memory)
{
    uint32_t newIndex = m_NextIndex++;
    lwgl::Handle newHdl = m_Lookup.Add(newIndex);
    T &newItem = m_Values[newIndex];
    m_LookupHandles[newIndex] = newHdl;

    out_memory = &newItem.m_Value;

    return newHdl;
}

template<typename T>
void lwgl::data::PackedArray<T>::Remove(const lwgl::Handle &handle)
{
    --m_NextIndex;
    uint32_t valueIndex = m_Lookup.Remove(handle);

    m_Values[valueIndex] = m_Values[m_NextIndex];
    m_LookupHandles[valueIndex] = m_LookupHandles[m_NextIndex];

    m_Lookup.Update(m_LookupHandles[valueIndex], valueIndex);
}

template<typename T>
T* lwgl::data::PackedArray<T>::Get(const lwgl::Handle &handle) const
{
    uint32_t index = m_Lookup.Get(handle);
    return &m_Values[index];
}

template<typename T>
T* lwgl::data::PackedArray<T>::GetIterator()
{
    return m_Values;
}
