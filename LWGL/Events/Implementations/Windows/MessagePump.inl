#pragma once

#include <pch.h>
#include "LWGL/Events/MessagePump.h"

int lwgl::events::PumpMessages()
{
    MSG msg;
    msg.message = WM_NULL;
    PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}
