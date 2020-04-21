#pragma once

#include <assert.h>

using namespace lwgl;
using namespace resources;

template<typename T>
RefCountedObject<T>::~RefCountedObject()
{
    assert(m_RefCount == 0);
}

template<typename T>
uint32_t RefCountedObject<T>::AddRef()
{
    return ++m_RefCount;
}

template<typename T>
uint32_t RefCountedObject<T>::Release()
{
    assert(m_RefCount > 0);
    --m_RefCount;

    if (m_RefCount == 0)
    {
        delete(static_cast<T*>(this));
    }

    return m_RefCount;
}

template<typename T>
RefCountedObject<T>::RefCountedObject()
    : m_RefCount(1)
{

}
