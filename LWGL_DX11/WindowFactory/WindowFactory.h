#pragma once

using namespace lwgl;

extern "C"
{
    EXPORT_SYMBOL
    WindowHandle CreateNewWindow(AppHandle owner, uint32_t width, uint32_t height);
}