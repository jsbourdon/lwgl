#pragma once

namespace lwgl
{
    namespace descriptors
    {
        enum class ShaderType
        {
            Unknonwn = -1,
            VertexShader,
            FragmentShader,
            EnumCount
        };

        struct ShaderDescriptor
        {
            ShaderType      Type;
            wchar_t*        FilePath;
            char*           EntryPoint;
            const char*     DebugName;
        };
    }
}