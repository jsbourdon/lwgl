#include <pch.h>
#include "GfxCommandList.h"
#include "LWGL/Device/GpuDevice.h"
#include "LWGL_Common/Descriptors/ViewportDescriptor.h"
#include "LWGL_Common/Descriptors/ScissorRectDescriptor.h"

using namespace lwgl;
using namespace lwgl::command;
using namespace lwgl::descriptors;

GfxCommandList::GfxCommandList(GpuDevice *pDevice, size_t byteSize)
    : m_pDevice(pDevice)
    , m_CmdAllocator(byteSize)
{

}

VertexBufferHandle* GfxCommandList::SetupInputAssembler(IndexBufferHandle indexBufferHdl, uint32_t vertexBufferCount)
{
    size_t handlesByteSize = vertexBufferCount * sizeof(VertexBufferHandle);
    GfxCommandPacket &cmd = AllocateCmdPacket(handlesByteSize);
    cmd.m_PacketType = CommandPacketType::IA_Setup;

    IASetup &setup = cmd.m_IASetup;
    setup.m_IndexBuffer = indexBufferHdl;
    setup.m_VertexBufferCount = vertexBufferCount;

    return reinterpret_cast<VertexBufferHandle*>(cmd.m_SetupAdditionalData);
}

TextureHandle* GfxCommandList::SetupRenderTargets(uint32_t count)
{
    size_t handlesByteSize = count * sizeof(TextureHandle);
    GfxCommandPacket &cmd = AllocateCmdPacket(handlesByteSize);
    cmd.m_PacketType = CommandPacketType::RT_Setup;

    RenderTargetSetup &setup = cmd.m_RTSetup;
    setup.m_RenderTargetCount = count;

    return reinterpret_cast<VertexBufferHandle*>(cmd.m_SetupAdditionalData);
}

void GfxCommandList::SetupDepthStencil(TextureHandle depthStencilHdl, uint32_t stencilRef)
{
    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::DS_Setup;

    DepthStencilSetup &setup = cmd.m_DSSetup;
    setup.m_DepthStencil = depthStencilHdl;
    setup.m_StencilRef = stencilRef;
}

void GfxCommandList::SetupOMBlendFactors(const float factors[4])
{
    using ArrayType = decltype(factors);
    using ArrayUnderlyingType = std::remove_all_extents_t<ArrayType>;
    static constexpr size_t copyByteSize = std::extent_v<ArrayType> * sizeof(ArrayUnderlyingType);

    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::BlendFactor_Setup;

    BlendFactorSetup &setup = cmd.m_BlendFactorSetup;
    memcpy(setup.m_BlendFactors, factors, copyByteSize);
}

ViewportDescriptor* GfxCommandList::SetupViewports(uint32_t count)
{
    size_t allocSize = count * sizeof(ViewportDescriptor);
    GfxCommandPacket &cmd = AllocateCmdPacket(allocSize);
    cmd.m_PacketType = CommandPacketType::Viewport_Setup;
    cmd.m_ViewportSetup.m_ViewportCount = count;

    return reinterpret_cast<ViewportDescriptor*>(cmd.m_SetupAdditionalData);
}

ScissorRectDescriptor* GfxCommandList::SetupScissorRects(uint32_t count)
{
    size_t allocSize = count * sizeof(ScissorRectDescriptor);
    GfxCommandPacket &cmd = AllocateCmdPacket(allocSize);
    cmd.m_PacketType = CommandPacketType::Scissor_Setup;
    cmd.m_ScissorSetup.m_ScissorRectCount = count;

    return reinterpret_cast<ScissorRectDescriptor*>(cmd.m_SetupAdditionalData);
}

void GfxCommandList::SetPipeline(GfxPipelineHandle pipeHdl, RenderPassHandle passHdl)
{
    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::Pipe_Setup;
    PipeSetup &setup = cmd.m_PipeSetup;
    setup.m_Pipeline = pipeHdl;
    setup.m_RenderPass = passHdl;
}

DescriptorHeapHandle* GfxCommandList::SetDescriptorHeaps(uint32_t count)
{
    size_t allocSize = sizeof(DescriptorHeapHandle) * count;
    GfxCommandPacket &cmd = AllocateCmdPacket(allocSize);
    cmd.m_PacketType = CommandPacketType::DescHeap_Setup;
    DescHeapSetup &setup = cmd.m_DescHeapSetup;
    setup.m_DescriptorHeapCount = count;

    return reinterpret_cast<DescriptorHeapHandle*>(cmd.m_SetupAdditionalData);
}

DescTables GfxCommandList::SetupDescriptorTables(uint32_t count)
{
    size_t allocSize = (sizeof(DescriptorTableHandle) + sizeof(uint32_t)) * count;
    GfxCommandPacket &cmd = AllocateCmdPacket(allocSize);
    cmd.m_PacketType = CommandPacketType::DescTable_Setup;
    cmd.m_DescTableSetup.m_DescriptorTableCount = count;

    DescTables tables;
    tables.m_Tables = reinterpret_cast<DescriptorTableHandle*>(cmd.m_SetupAdditionalData);
    tables.m_Indices = reinterpret_cast<uint32_t*>(tables.m_Tables + count);

    return tables;
}

BarrierSet GfxCommandList::SetupBarrierMemory(
    uint32_t textureTransitionCount,
    uint32_t bufferTransitionCount,
    uint32_t textureAliasingCount,
    uint32_t bufferAliasingCount,
    uint32_t textureUAVCount,
    uint32_t bufferUAVCount)
{
    size_t allocSize =
        textureTransitionCount * sizeof(TextureTransitionBarrier) +
        bufferTransitionCount * sizeof(BufferTransitionBarrier) +
        textureAliasingCount * sizeof(TextureAliasingBarrier) +
        bufferAliasingCount * sizeof(BufferAliasingBarrier) +
        textureUAVCount * sizeof(Handle) +
        bufferUAVCount * sizeof(Handle);

    GfxCommandPacket &cmd = AllocateCmdPacket(allocSize);
    cmd.m_PacketType = CommandPacketType::Barriers;

    BarrierSetup &setup = cmd.m_BarrierSetup;
    setup.m_TextureTransitionBarrierCount = textureTransitionCount;
    setup.m_BufferTransitionBarrierCount = bufferTransitionCount;
    setup.m_TextureAliasingBarrierCount = textureAliasingCount;
    setup.m_BufferAliasingBarrierCount = bufferAliasingCount;
    setup.m_TextureUAVBarrierCount = textureUAVCount;
    setup.m_BufferUAVBarrierCount = bufferUAVCount;

    TextureTransitionBarrier *pTextureTransitions = reinterpret_cast<TextureTransitionBarrier*>(cmd.m_SetupAdditionalData);
    BufferTransitionBarrier *pBufferTransitions = reinterpret_cast<BufferTransitionBarrier*>(pTextureTransitions + textureTransitionCount);
    TextureAliasingBarrier *pTextureAlias = reinterpret_cast<TextureAliasingBarrier*>(pBufferTransitions + bufferTransitionCount);
    BufferAliasingBarrier *pBufferAlias = reinterpret_cast<BufferAliasingBarrier*>(pTextureAlias + textureAliasingCount);
    Handle *pTextureUAV = reinterpret_cast<Handle*>(pBufferAlias + bufferAliasingCount);
    Handle *pBufferUAV = reinterpret_cast<Handle*>(pTextureUAV + textureUAVCount);

    BarrierSet set;
    set.m_pTextureTransitions = textureTransitionCount > 0 ? pTextureTransitions : nullptr;
    set.m_pBufferTransitions = bufferTransitionCount > 0 ? pBufferTransitions : nullptr;
    set.m_pTextureAliasing = textureAliasingCount > 0 ? pTextureAlias : nullptr;
    set.m_pBufferAliasing = bufferAliasingCount > 0 ? pBufferAlias : nullptr;
    set.m_pTextureUAV = textureUAVCount > 0 ? pTextureUAV : nullptr;
    set.m_pBufferUAV = bufferUAVCount > 0 ? pBufferUAV : nullptr;

    return set;
}

void GfxCommandList::ClearRenderTarget(TextureHandle hdl)
{
    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::Clear;
    cmd.m_ClearOp.m_Target = hdl;
}

void GfxCommandList::DrawIndexed(uint32_t indexCount, uint32_t startIndex, uint32_t baseVertexOffset)
{
    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::DrawIndexed;
    DrawIndexedOp &setup = cmd.m_DrawIndexedOp;
    setup.m_IndexCount = indexCount;
    setup.m_StartIndex = startIndex;
    setup.m_BaseVertexOffset = baseVertexOffset;
}

void GfxCommandList::Close()
{
    GfxCommandPacket &cmd = AllocateCmdPacket(0);
    cmd.m_PacketType = CommandPacketType::EOS;
    m_pDevice->CloseCommandList(m_CmdBufferHdl, m_CmdAllocator.GetFrameStartAddress());
}

void GfxCommandList::Reset()
{
    m_CmdAllocator.Reset();
}

size_t GfxCommandList::GetTotalAllocSize(size_t additionalAlloc)
{
    static constexpr size_t s_AdditionalDataSize = sizeof(std::declval<GfxCommandPacket>().m_SetupAdditionalData);

    size_t reducedAddAlloc = additionalAlloc - s_AdditionalDataSize;
    size_t additionalBytes = (additionalAlloc > s_AdditionalDataSize) ? reducedAddAlloc : 0;
    return sizeof(GfxCommandPacket) + additionalBytes;
}

GfxCommandPacket& GfxCommandList::AllocateCmdPacket(size_t additionalAlloc)
{
    size_t allocSize = GetTotalAllocSize(additionalAlloc);
    return *reinterpret_cast<GfxCommandPacket*>(m_CmdAllocator.Allocate(allocSize, alignof(GfxCommandPacket)));
}
