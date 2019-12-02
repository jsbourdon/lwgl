#pragma once

#include "LWGL/Library/LibraryLoader.h"
#include "LWGL_Common/Memory/Memory.h"
#include "LWGL_Common/Core/Defines.h"
#include "LWGL/Commands/GfxCommandList.h"

using namespace lwgl::external;
using namespace lwgl::resources;
using namespace lwgl::memory;
using namespace lwgl::command;

namespace lwgl
{
    enum class GfxPlatform;

    class GpuDevice
    {
    public:

        static GpuDevice* CreateDevice(GfxPlatform platform);
        static void DestroyDevice(GpuDevice *pDevice);

        CommandQueueHandle CreateCommandQueue(CommandQueueType type);
        void DestroyCommandQueue(CommandQueueHandle cmdQueueHdl);

        GfxCommandList CreateGfxCommandList(size_t memoryByteSize = MiB(1));
        void DestroyGfxCommandList(GfxCommandList list);

    private:

        GpuDevice() = default;
        ~GpuDevice();

        bool Init(GfxPlatform platform);

        ExternalFunctions m_LibFunctions {};
        GpuDeviceHandle m_DeviceHandle { Handle_NULL };
    };
}