#pragma once

#include "LWGL_Common/Commands/GfxCommandBuffer.h"
#include "LWGL_Common/Sync/Barrier.h"
#include "Salvation_Common/Memory/FrameMemoryAllocator.h"

using namespace salvation::memory;
using namespace lwgl::command;
using namespace lwgl::sync;

namespace lwgl
{
    class GpuDevice;

    namespace descriptors
    {
        enum class PrimitiveTopology;
        struct ViewportDescriptor;
        struct ScissorRectDescriptor;
    }

    using namespace lwgl::descriptors;

    namespace command
    {
        class GfxCommandQueue;

        struct DescTables
        {
            DescriptorTableHandle*  m_Tables;
            uint32_t*               m_Indices;
        };

        class GfxCommandList
        {
            friend class lwgl::GpuDevice;
            friend class lwgl::command::GfxCommandQueue;

        public:

            // Input Assembler
            VertexBufferHandle* SetupInputAssembler(IndexBufferHandle indexBufferHdl, uint32_t vertexBufferCount);

            // Output Merger
            TextureHandle* SetupRenderTargets(uint32_t count);
            void SetupDepthStencil(TextureHandle depthStencilHdl, uint32_t stencilRef);
            void SetupOMBlendFactors(const float factors[4]);

            // Rasterizer
            ViewportDescriptor* SetupViewports(uint32_t count);
            ScissorRectDescriptor* SetupScissorRects(uint32_t count);

            // Pipeline and Resources
            void SetPipeline(GfxPipelineHandle pipeHdl, RenderPassHandle passHdl);
            DescriptorHeapHandle* SetDescriptorHeaps(uint32_t count);
            DescTables SetupDescriptorTables(uint32_t count);

            // Barriers
            BarrierSet SetupBarrierMemory(
                uint32_t textureTransitionCount, 
                uint32_t bufferTransitionCount, 
                uint32_t textureAliasingCount, 
                uint32_t bufferAliasingCount, 
                uint32_t textureUAVCount, 
                uint32_t bufferUAVCount);

            // Operations
            void ClearRenderTarget(TextureHandle hdl);
            void DrawIndexed(uint32_t indexCount, uint32_t startIndex, uint32_t baseVertexOffset);

            // End of processing
            void Close();
            void Reset();

        private:

            static size_t       GetTotalAllocSize(size_t additionalAlloc);
            GfxCommandPacket&   AllocateCmdPacket(size_t additionalAlloc);

            GfxCommandList(GpuDevice *pDevice, size_t byteSize);
            ~GfxCommandList() = default;

            GpuDevice*              m_pDevice;
            FrameMemoryAllocator    m_CmdAllocator;
            Handle                  m_CmdBufferHdl { Handle_NULL };
        };
    }
}