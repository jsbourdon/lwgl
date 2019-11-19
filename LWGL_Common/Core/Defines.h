#pragma once

#define STRINGIFY(value) STRINGIFY2(value)
#define STRINGIFY2(value) #value
#define CONCAT(value0, value1) CONCAT2(value0, value1)
#define CONCAT2(value0, value1) value0##value1

#ifdef _MSC_VER

    #define EXPORT_SYMBOL __declspec(dllexport)
    #define PLATFORM_WINDOWS 1
    #define INCLUDE_IMPLEMENTATION(fileName) STRINGIFY(CONCAT(CONCAT(Implementations/Windows/, fileName), .inl))

#endif