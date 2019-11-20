#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"

namespace lwgl
{
    namespace factory
    {
        lwgl::WindowHandle CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height);
        void DisplayWindow(const lwgl::WindowHandle &hwnd);
        void HideWindow(const lwgl::WindowHandle &hwnd);
    }
}
