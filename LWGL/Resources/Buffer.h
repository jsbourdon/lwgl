#pragma once

#include "../Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class Buffer : public RefCountedObject<Buffer>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            Buffer();
            ~Buffer();

        private:

            ID3D11Buffer*               m_pD3DBuffer;
            ID3D11ShaderResourceView*   m_pD3DBufferSRV;
        };
    }
}