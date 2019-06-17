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

            InputLayoutDescriptor       InputLayout;
            ShaderDescriptor            VertexShader;
            ShaderDescriptor            FragmentShader;
            BlendStateDescriptor        BlendState;
            DepthStencilStateDescriptor DepthStencilState;
            RasterizerStateDescriptor   RasterizerState;
            PixelFormat                 RenderTargetFormats[s_OMMaxRenderTargetCount];
            PixelFormat                 DepthFormat;
        };
    }
}
