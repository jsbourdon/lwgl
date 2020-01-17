#pragma once

#include <stdint.h>
#include <utility>
#include <limits>
#include "Salvation_Common/Core/Defines.h"
#include "Salvation_Common/Math/Math.h"
#include "Salvation_Common/Handle/Handle.h"

using namespace salvation;

namespace lwgl
{
    using TextureHandle =           ParentResourceHandle<0x1F>;
    using Texture2DHandle =         ChildResourceHandle<TextureHandle, 0x01>;
    using Texture3DHandle =         ChildResourceHandle<TextureHandle, 0x02>;
    using TextureCubeHandle =       ChildResourceHandle<TextureHandle, 0x04>;
    using TextureArrayHandle =      ChildResourceHandle<TextureHandle, 0x08>;
    using TextureSubResource =      ChildResourceHandle<TextureHandle, 0x10>;

    using BufferHandle =            ParentResourceHandle<0x1F>;
    using ConstantBufferHandle =    ChildResourceHandle<BufferHandle, 0x01>;
    using RWBufferHandle =          ChildResourceHandle<BufferHandle, 0x02>;
    using StructuredBufferHandle =  ChildResourceHandle<BufferHandle, 0x04>;
    using VertexBufferHandle =      ChildResourceHandle<BufferHandle, 0x08>;
    using IndexBufferHandle =       ChildResourceHandle<BufferHandle, 0x10>;

    typedef Handle DescriptorTableHandle;
    typedef Handle DescriptorHeapHandle;
    typedef Handle RenderPassHandle;
    typedef Handle GfxPipelineHandle;

    typedef uintptr_t WindowHandle;
    typedef uintptr_t AppHandle;
    typedef uintptr_t LibraryHandle;

    typedef uintptr_t GpuDeviceHandle;
    typedef uintptr_t CommandQueueHandle;
    typedef uintptr_t CommandBufferHandle;
    typedef uintptr_t FenceHandle;
}