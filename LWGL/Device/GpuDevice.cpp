#include <pch.h>
#include "GpuDevice.h"
#include "GfxPlatform.h"
#include "LWGL/Library/LibraryLoader.h"

using namespace lwgl;
using namespace lwgl::external;

GpuDevice* GpuDevice::CreateDevice(GfxPlatform platform)
{
    GpuDevice *pDevice = new GpuDevice();

    if (!pDevice->Init(platform))
    {
        delete pDevice;
        pDevice = nullptr;
    }

    return pDevice;
}

bool GpuDevice::Init(GfxPlatform platform)
{
    m_LibFunctions = LibraryLoader::LoadExternalFunctions(platform);
    m_DeviceHandle = m_LibFunctions.CreateGfxDevice();

    return m_DeviceHandle != Handle_NULL;
}

void GpuDevice::DestroyDevice(GpuDevice *pDevice)
{
    delete pDevice;
}

GpuDevice::~GpuDevice()
{
    m_LibFunctions.DestroyGfxDevice(m_DeviceHandle);
    LibraryLoader::UnloadExternalFunctions(m_LibFunctions);
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

GfxCommandList GpuDevice::CreateGfxCommandList(size_t memoryByteSize)
{
    GfxCommandList cmdList;
    return cmdList;
}
