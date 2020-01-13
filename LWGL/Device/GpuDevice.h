#pragma once

#include "LWGL/Library/LibraryLoader.h"
#include "Salvation_Common/Memory/Memory.h"
#include "Salvation_Common/Core/Defines.h"
#include "Salvation_Common/DataStructures/PackedArray.h"
#include "LWGL/Commands/GfxCommandList.h"

using namespace salvation::memory;
using namespace lwgl::external;
using namespace lwgl::resources;
using namespace lwgl::command;

namespace lwgl
{
    enum class GfxPlatform;

    class GpuDevice
    {
    public:

        static GpuDevice* CreateDevice();
        static void DestroyDevice(GpuDevice *pDevice);

        CommandQueueHandle CreateCommandQueue(CommandQueueType type);
        void DestroyCommandQueue(CommandQueueHandle cmdQueueHdl);

        GfxCommandList* CreateGfxCommandList(size_t memoryByteSize = MiB(1));
        void CloseCommandList(Handle cmdBufferHdl, void *pPackets) const;
        void DestroyGfxCommandList(GfxCommandList *pList);

    private:

        static constexpr uint32_t s_MaxCmdBufferCount = 100;

        GpuDevice();
        ~GpuDevice();

        bool Init();

        PackedArray<GfxCommandBuffer>   m_CmdBuffers;
        GpuDeviceFunctions              m_LibFunctions {};
        GpuDeviceHandle                 m_DeviceHandle { Handle_NULL };
    };
}