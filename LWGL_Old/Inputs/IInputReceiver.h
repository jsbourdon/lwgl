#pragma once

#include <pch.h>

namespace lwgl
{
    namespace input
    {
        class IInputReceiver
        {
        public:

            virtual void OnKeyDown(uint32_t keyCode, bool firstDown) = 0;
            virtual void OnKeyUp(uint32_t keyCode) = 0;
        };
    }
}