#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL_Common/Resources/ResourceTypes.h"

namespace lwgl
{
    namespace external
    {
        typedef bool(*InitLibFnctPtr)();

        namespace gfx
        {
            typedef GpuDeviceHandle(*CreateGfxDeviceFnctPtr)();
            typedef void (*DestroyGfxDeviceFnctPtr)(lwgl::GpuDeviceHandle hdl);
            typedef CommandQueueHandle(*CreateCommandQueueFnctPtr)(lwgl::resources::CommandQueueType);
            typedef void (*DestroyCommandQueueFnctPtr)(lwgl::CommandQueueHandle hdl);
        }
    }
}