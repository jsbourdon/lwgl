#pragma once

#include "../3rd/DXUT/Core/DXUT.h"

namespace lwgl
{
    namespace core
    {
        constexpr float PI = DirectX::XM_PI;
        constexpr float TWO_PI = DirectX::XM_2PI;
        constexpr float ONE_ON_PI = DirectX::XM_1DIVPI;
        constexpr float ONE_ON_TWO_PI = DirectX::XM_1DIV2PI;
        constexpr float PI_ON_TWO = DirectX::XM_PIDIV2;
        constexpr float PI_ON_FOUR = DirectX::XM_PIDIV4;

        constexpr size_t MAX_RENDERTARGET_COUNT = 8;
    }
}