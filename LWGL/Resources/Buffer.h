#pragma once

#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class Buffer : public RefCountedResource<Buffer>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            Buffer();
            ~Buffer();

        private:

            ID3D11Buffer*   m_pD3DBuffer;
        };
    }
}