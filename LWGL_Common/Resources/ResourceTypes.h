#pragma once

namespace lwgl
{
    namespace resources
    {
        enum class CommandQueueType
        {
            Graphic,
            Compute,
            Copy
        };

        enum class CommandBufferType
        {
            Graphic,
            GraphicBundle,
            Compute,
            Copy
        };
    }
}