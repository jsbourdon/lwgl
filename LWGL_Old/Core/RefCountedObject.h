#pragma once

#include <pch.h>

namespace lwgl
{
    namespace resources
    {
        template<typename T>
        class RefCountedObject
        {
        public:

            virtual ~RefCountedObject();
            uint32_t AddRef();
            uint32_t Release();

        protected:

            typedef RefCountedObject<T> base;

            RefCountedObject();
            
        private:

            uint32_t m_RefCount;
        };
    }
}

#include "RefCountedObject.inl"