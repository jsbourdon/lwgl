#pragma once

using namespace lwgl;
using namespace resources;

template<typename T>
RefCountedResource<T>::~RefCountedResource()
{
	assert(m_RefCount == 0);
}

template<typename T>
uint32_t RefCountedResource<T>::AddRef()
{
	++m_RefCount;
}

template<typename T>
uint32_t RefCountedResource<T>::Release()
{
	assert(m_RefCount > 0);
	--m_RefCount;

	if (m_RefCount == 0)
	{
		delete(static_cast<T*>(this));
	}
}

template<typename T>
RefCountedResource<T>::RefCountedResource()
	: m_RefCount(1)
{

}
