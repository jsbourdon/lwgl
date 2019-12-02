#pragma once

#include <stdint.h>

#define VALIDATE_HANDLES _DEBUG

namespace lwgl
{
    struct Handle
    {
        Handle(uintptr_t value)
        {
            m_Value = static_cast<uint32_t>(value & 0xFFFFFFFF);
    #if VALIDATE_HANDLES
            m_Generation = static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF);
    #endif
        }

        bool operator==(Handle other)
        {
            return
                m_Value == other.m_Value
            #if VALIDATE_HANDLES
                && m_Generation == other.m_Generation
            #endif
                ;
        }

        bool operator==(uintptr_t other)
        {
            return
                m_Value == static_cast<uint32_t>(other & 0xFFFFFFFF)
            #if VALIDATE_HANDLES
                && m_Generation == static_cast<uint32_t>((other >> 32) & 0xFFFFFFFF)
            #endif
                ;
        }

        operator uintptr_t() 
        { 
            return (static_cast<uintptr_t>(m_Generation) << 32 | static_cast<uintptr_t>(m_Value));
        }

        uint32_t m_Value;

    #if VALIDATE_HANDLES
        uint32_t m_Generation { 0 };
    #endif
    };

    #define ParentResourceHandle(Name)                  \
        struct Name                                     \
        {                                               \
            Name(uintptr_t value) : m_Handle(value) {}  \
            Name(Handle handle) : m_Handle(handle) {}   \
            Handle m_Handle;                            \
        }

    #define ChildResourceHandle(Name, ParentName)           \
        struct Name : ParentName                            \
        {                                                   \
            Name(uintptr_t value) : ParentName(value) {}    \
            Name(Handle handle) : ParentName(handle) {}     \
            operator ParentName() { return m_Handle; }      \
        }

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

    typedef uintptr_t GpuDeviceHandle;
    typedef uintptr_t CommandQueueHandle;
    typedef uintptr_t GfxCommandBufferHandle;

    static const uintptr_t Handle_NULL = -1;
}