#pragma once

#include <stdafx.h>

namespace lwgl
{
    namespace resources
    {
        template<typename T>
        class RefCountedResource
        {
        public:

            virtual ~RefCountedResource();
            uint32_t AddRef();
            uint32_t Release();

        protected:

            typedef RefCountedResource<T> base;

            RefCountedResource();
            
        private:

            uint32_t m_RefCount;
        };
    }
}

#include "RefCountedResource.inl"