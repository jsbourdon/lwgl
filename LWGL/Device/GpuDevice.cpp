#include <pch.h>
#include "GpuDevice.h"
#include "GfxPlatform.h"
#include "LWGL/Library/LibraryLoader.h"
#include "LWGL_Common/Memory/ThreadHeapAllocator.h"

using namespace lwgl;
using namespace lwgl::external;
using namespace lwgl::memory;

GpuDevice::GpuDevice()
    : m_CmdBuffers(s_MaxCmdBufferCount)
{

}

GpuDevice::~GpuDevice()
{
    m_LibFunctions.DestroyGfxDevice(m_DeviceHandle);
}

GpuDevice* GpuDevice::CreateDevice()
{
    GpuDevice *pDevice = new GpuDevice();

    if (!pDevice->Init())
    {
        delete pDevice;
        pDevice = nullptr;
    }

    return pDevice;
}

bool GpuDevice::Init()
{
    m_LibFunctions = LibraryLoader::LoadGpuDeviceFunctions();
    m_DeviceHandle = m_LibFunctions.CreateGfxDevice();

    return m_DeviceHandle != Handle_NULL;
}

void GpuDevice::DestroyDevice(GpuDevice *pDevice)
{
    delete pDevice;
}

CommandQueueHandle GpuDevice::CreateCommandQueue(CommandQueueType type)
{
    LWGL_ASSERT(type == CommandQueueType::Graphic, "Unsupported Command Queue type");
    return m_LibFunctions.CreateCommandQueue(m_DeviceHandle, type);
}

void GpuDevice::DestroyCommandQueue(CommandQueueHandle cmdQueueHdl)
{
    m_LibFunctions.DestroyCommandQueue(cmdQueueHdl);
}

GfxCommandList* GpuDevice::CreateGfxCommandList(size_t memoryByteSize)
{
    GfxCommandList *pCmdList = new GfxCommandList(this, memoryByteSize);

    GfxCommandBuffer cmdBuffer;
    cmdBuffer.m_CmdbufferHdl = m_LibFunctions.CreateCommandBuffer(m_DeviceHandle, CommandBufferType::Graphic);
    pCmdList->m_CmdBufferHdl = m_CmdBuffers.Add(cmdBuffer);

    return pCmdList;
}

void GpuDevice::CloseCommandList(Handle cmdBufferHdl, void *pPackets) const
{
    GfxCommandBuffer *pCmdBuffer = m_CmdBuffers.Get(cmdBufferHdl);
    pCmdBuffer->m_pPackets = pPackets;
}

void GpuDevice::DestroyGfxCommandList(GfxCommandList *pList)
{
    m_CmdBuffers.Remove(pList->m_CmdBufferHdl);
    delete pList;
}
