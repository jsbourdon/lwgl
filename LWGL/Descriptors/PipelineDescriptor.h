#pragma once

#include "InputLayoutDescriptor.h"
#include "BlendStateDescriptor.h"
#include "ShaderDescriptor.h"
#include "DepthStencilStateDescriptor.h"
#include "PixelFormats.h"


namespace lwgl
{
    using namespace resources;

    namespace descriptors
    {
        struct PipelineDescriptor
        {
            InputLayoutDescriptor       InputLayout;
            ShaderDescriptor            VertexShader;
            ShaderDescriptor            FragmentShader;
            BlendStateDescriptor        BlendState;
            DepthStencilStateDescriptor DepthStencilState;
        };
    }
}
