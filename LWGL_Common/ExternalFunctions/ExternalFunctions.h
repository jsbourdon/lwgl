#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace external
    {
        typedef WindowHandle (*CreateWindowFnctPtr)(AppHandle owner, uint32_t width, uint32_t height);
        typedef void (*ProcessWindowSystemEventsFnctPtr)();
    }
}