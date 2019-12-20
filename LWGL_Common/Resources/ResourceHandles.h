#pragma once

#include <stdint.h>
#include <utility>
#include "LWGL_Common/Math/Math.h"

#define VALIDATE_HANDLES _DEBUG

namespace lwgl
{
    static const uintptr_t Handle_NULL = UINTPTR_MAX;

    struct alignas(sizeof(uintptr_t)) Handle
    {
        Handle()
        #if VALIDATE_HANDLES
            : m_Value(UINT32_MAX)
            , m_Generation(UINT32_MAX)
        #else
            : m_Value(UINTPTR_MAX)
        #endif
        {

        }

        Handle(uintptr_t value)
        #if VALIDATE_HANDLES
            : m_Value(static_cast<uint32_t>(value & 0xFFFFFFFF))
            , m_Generation(static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF))
        #else
            : m_Value(value)
        #endif
        {
        }

        bool operator==(Handle other)
        {
            return m_Value == other.m_Value
        #if VALIDATE_HANDLES
                && m_Generation == other.m_Generation
        #endif
                ;
        }

        bool operator==(uintptr_t other)
        {
        #if VALIDATE_HANDLES
            return
                m_Value == static_cast<uint32_t>(other & 0xFFFFFFFF)
                && m_Generation == static_cast<uint32_t>((other >> 32) & 0xFFFFFFFF);
        #else
            return m_Value == other;
        #endif
        }

        operator uintptr_t() 
        { 
        #if VALIDATE_HANDLES
            return (static_cast<uintptr_t>(m_Generation) << 32 | static_cast<uintptr_t>(m_Value));
        #else
            return m_Value;
        #endif
        }

    #if VALIDATE_HANDLES
        uint32_t m_Value { UINT32_MAX };
        uint32_t m_Generation { UINT32_MAX };
    #else
        uintptr_t m_Value { UINTPTR_MAX };
    #endif
    };

    static_assert(sizeof(Handle) == sizeof(uintptr_t));

    typedef decltype(std::declval<Handle>().m_Value) ResourceTypeMask;

    template<ResourceTypeMask TypeMask>
    struct ParentResourceHandle
    {
        static constexpr ResourceTypeMask s_TypeMask = TypeMask;
        static constexpr ResourceTypeMask s_SubTypeOffset = lwgl::math::CountLeadingZeros(TypeMask);
        static constexpr ResourceTypeMask s_SubTypeMask = TypeMask << s_SubTypeOffset;

        ParentResourceHandle() : m_Handle(Handle_NULL) {}
        ParentResourceHandle(uintptr_t value) : m_Handle(value) {}
        ParentResourceHandle(Handle handle) : m_Handle(handle) {}
            
        template<typename T>
        inline bool Is() const
        {
            return (((m_Handle.m_Value & s_SubTypeMask) >> s_SubTypeOffset) & T::s_TypeMask) == T::s_TypeMask; 
        }
            
        inline Handle TypeLessHandle() const
        {
            Handle newHdl(m_Handle);
            newHdl.m_Value = m_Handle.m_Value & ~s_SubTypeMask;
            return newHdl;
        }

        Handle m_Handle;
    };

    template<typename ParentType, ResourceTypeMask TypeMask,
        std::enable_if_t<std::is_base_of<ParentResourceHandle<ParentType::s_TypeMask>, ParentType>::value, int> = 0>
    struct ChildResourceHandle : ParentType
    {
        static constexpr ResourceTypeMask s_TypeMask = TypeMask;

        ChildResourceHandle() : ParentType(Handle_NULL) {}
        ChildResourceHandle(uintptr_t value) : ParentType(value) {}
        ChildResourceHandle(Handle handle) : ParentType(handle) {}
        operator ParentType() { return ParentType::m_Handle; }
            
        static void InsertType(Handle &hdl)
        {
            ParentType::m_Handle.m_Value |= ParentType::s_SubTypeMask;
        }

        //static_assert(sizeof(Handle) == sizeof(ChildResourceHandle));
    };

    // #todo Remove when tests are completed
    struct TestType
    {
        TestType() : m_Handle(Handle_NULL) {}
        TestType(uintptr_t value) : m_Handle(value) {}
        TestType(Handle handle) : m_Handle(handle) {}

        Handle m_Handle;
    };

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