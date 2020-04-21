#pragma once

namespace lwgl
{
    namespace descriptors
    {
        enum class ComparisonFunction
        {
            NEVER,
            LESS,
            EQUAL,
            LESS_EQUAL,
            GREATER,
            NOT_EQUAL,
            GREATER_EQUAL,
            ALWAYS,
            EnumCount
        };

        enum class StencilOperation
        {
            KEEP,
            ZERO,
            REPLACE,
            INCR_SAT,
            DECR_SAT,
            INVERT,
            INCR,
            DECR,
            EnumCount
        };

        struct StencilOperationDescriptor
        {
            StencilOperation StencilFailOp;
            StencilOperation DepthFailOp;
            StencilOperation DepthStencilPassOp;
            ComparisonFunction Function;
        };

        struct DepthStencilStateDescriptor
        {
            StencilOperationDescriptor FrontFaceStencil;
            StencilOperationDescriptor BackFaceStencil;
            ComparisonFunction DepthFunction;
            uint8_t StencilReadMask;
            uint8_t StencilWriteMask;
            bool IsDepthTestEnabled;
            bool IsDepthWriteEnabled;
            bool IsStencilEnabled;
        };
    }
}