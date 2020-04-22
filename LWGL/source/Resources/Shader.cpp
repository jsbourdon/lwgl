#include <pch.h>
#include "Resources/Shader.h"
#include "3rd/DXUT/Optional/SDKmisc.h"

using namespace lwgl;
using namespace resources;

Shader::Shader()
    : base()
{

}

Shader::~Shader()
{
    if (m_Type == ShaderType::VertexShader)
    {
        SAFE_RELEASE(m_pVertexShader);
    }
    else if (m_Type == ShaderType::FragmentShader)
    {
        SAFE_RELEASE(m_pFragmentShader);
    }

    SAFE_RELEASE(m_pShaderBuffer);
}
