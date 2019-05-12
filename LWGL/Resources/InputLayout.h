#pragma once

#include <stdafx.h>
#include "RefCountedResource.h"

namespace lwgl
{
    namespace resources
    {
        class InputLayout : public RefCountedResource<InputLayout>
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