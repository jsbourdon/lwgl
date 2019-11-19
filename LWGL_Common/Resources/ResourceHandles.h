#pragma once

#define VALIDATE_HANDLES _DEBUG

namespace lwgl
{
    struct Handle
    {
        uint32_t m_Value;

    #if VALIDATE_HANDLES
        uint32_t m_Generation { 0 };
    #endif
    };

    #define ParentResourceHandle(Name)                  \
        struct Name                                     \
        {                                               \
            Name(Handle handle) : m_Handle(handle) {}   \
            Handle m_Handle;                            \
        }

    #define ChildResourceHandle(Name, ParentName) struct Name : ParentName { operator ParentName() { return m_Handle; } }

    ParentResourceHandle(TextureHandle);
    ChildResourceHandle(Texture2DHandle, TextureHandle);
    ChildResourceHandle(Texture3DHandle, TextureHandle);
    ChildResourceHandle(TextureCubeHandle, TextureHandle);
    ChildResourceHandle(TextureArrayHandle, TextureHandle);

    ParentResourceHandle(BufferHandle);
    ChildResourceHandle(ConstantBufferHandle, BufferHandle);
    ChildResourceHandle(RWBufferHandle, BufferHandle);
    ChildResourceHandle(StructuredBufferHandle, BufferHandle);
    ChildResourceHandle(VertexBufferHandle, BufferHandle);
    ChildResourceHandle(IndexBufferHandle, BufferHandle);

    typedef Handle DescriptorTableHandle;
    typedef Handle RenderPassHandle;
    typedef Handle GfxPipelineHandle;
    typedef Handle FrameBufferHandle;

    typedef uintptr_t WindowHandle;
    typedef uintptr_t AppHandle;
    typedef uintptr_t LibraryHandle;
}