#pragma once

#include "LWGL_Common/Resources/ResourceHandles.h"
#include "LWGL/Library/LibraryLoader.h"

using namespace lwgl::external;

namespace lwgl
{
    class GpuDevice;

    namespace command
    {
        class GfxCommandList;

        class GfxCommandQueue
        {
            friend class lwgl::GpuDevice;

        public:

            void ExecuteCommandLists(uint32_t count, const GfxCommandList **pLists);
            void SignalFence(FenceHandle hdl, uint64_t value);
            void WaitForFence(FenceHandle hdl, uint64_t value);

        private:

            GfxCommandQueue() = default;
            ~GfxCommandQueue() = default;

            CommandQueueHandle      m_CmdQueueHandle { Handle_NULL };
            CommandQueueFunctions   m_LibFunctions {};
        };
    }
}
