#pragma once

#include "InputLayoutDescriptor.h"
#include "BlendStateDescriptor.h"
#include "ShaderDescriptor.h"
#include "DepthStencilStateDescriptor.h"
#include "RasterizerStateDescriptor.h"
#include "PixelFormats.h"


namespace lwgl
{
    using namespace resources;

    namespace descriptors
    {
        struct PipelineDescriptor
        {
            static constexpr size_t s_OMMaxRenderTargetCount = 8;

            InputLayoutDescriptor       InputLayout {};
            ShaderDescriptor            VertexShader { nullptr, nullptr, nullptr, nullptr, ShaderType::VertexShader };
            ShaderDescriptor            FragmentShader { nullptr, nullptr, nullptr, nullptr, ShaderType::FragmentShader };
            BlendStateDescriptor        BlendState {};
            DepthStencilStateDescriptor DepthStencilState {};
            RasterizerStateDescriptor   RasterizerState {};
            PixelFormat                 RenderTargetFormats[s_OMMaxRenderTargetCount] {};
            PixelFormat                 DepthFormat {};
        };
    }
}
