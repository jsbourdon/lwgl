#pragma once

#include "Core/RefCountedObject.h"
#include "Descriptors/ShaderDescriptor.h"

namespace lwgl
{
    using namespace descriptors;

    namespace resources
    {
        class Shader final : public RefCountedObject<Shader>
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
                ID3D11VertexShader* m_pVertexShader { nullptr };
                ID3D11PixelShader*  m_pFragmentShader;
            };

            ID3DBlob*   m_pShaderBuffer { nullptr };
            ShaderType  m_Type {};
        };
    }
}