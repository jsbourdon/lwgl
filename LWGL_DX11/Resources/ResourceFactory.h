#pragma once

#include "LWGL_Common/Core/Defines.h"
#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Resources/ResourceTypes.h"

extern "C"
{
    EXPORT_SYMBOL
    lwgl::GpuDeviceHandle CreateGfxDevice();

    EXPORT_SYMBOL
    void DestroyGfxDevice(lwgl::GpuDeviceHandle hdl);

    EXPORT_SYMBOL
    lwgl::CommandQueueHandle CreateCommandQueue(lwgl::GpuDeviceHandle device, lwgl::resources::CommandQueueType type);

    EXPORT_SYMBOL
    void DestroyCommandQueue(lwgl::CommandQueueHandle hdl);
}