#pragma once

#include "LWGL_Common/Commands/GfxCommandBuffer.h"

using namespace lwgl::command;

namespace lwgl
{
    class GpuDevice;

    namespace command
    {
        class GfxCommandList
        {
            friend class lwgl::GpuDevice;

        public:

            

        private:

            GfxCommandList() = default;
            ~GfxCommandList() = default;

            GfxCommandBuffer m_CmdBuffer {};
        };
    }
}