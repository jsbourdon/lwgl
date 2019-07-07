#pragma once

#include <pch.h>
#include "../Core/RefCountedObject.h"

namespace lwgl
{
    namespace resources
    {
        class InputLayout final : public RefCountedObject<InputLayout>
        {
            friend base;
            friend class GfxDevice;
            friend class GfxDeviceContext;

        private:

            InputLayout();
            ~InputLayout();

        private:

            ID3D11InputLayout*  m_pLayout;
        };
    }
}