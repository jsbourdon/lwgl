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
            const wchar_t*  FilePath;
            const char*     EntryPoint;
            const char*     DebugName;
            ShaderType      Type;
        };
    }
}