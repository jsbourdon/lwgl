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
            const wchar_t*  FilePath { nullptr };
            const char*     Code { nullptr };
            const char*     EntryPoint { nullptr };
            const char*     DebugName { nullptr };
            ShaderType      Type { ShaderType::Unknonwn };
        };
    }
}