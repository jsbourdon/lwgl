#include <pch.h>
#include "Shader.h"
#include "../3rd/DXUT/Optional/SDKmisc.h"

using namespace lwgl;
using namespace resources;

Shader::Shader()
    : base()
    , m_pShaderBuffer(nullptr)
    , m_Type(ShaderType::Unknonwn)
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
