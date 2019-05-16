#pragma once

#include "../Core/RefCountedObject.h"
#include "../Descriptors/ShaderDescriptor.h"

namespace lwgl
{
    using namespace descriptors;

    namespace resources
    {
        class Shader : public RefCountedObject<Shader>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            Shader();
            ~Shader();

        private:

            union
            {
                ID3D11VertexShader* m_pVertexShader;
                ID3D11PixelShader*  m_pFragmentShader;
            };

            ID3DBlob*   m_pShaderBuffer;
            ShaderType  m_Type;
        };
    }
}