#pragma once

#include <stdio.h>
#include "LibraryLoader.h"
#include "LWGL_Common/Memory/Memory.h"

using namespace lwgl;
using namespace lwgl::external;

LibraryHandle LibraryLoader::LoadDynamicLibrary(const wchar_t *name)
{
    const size_t strLength = wcslen(name);
    const size_t bufferSize = (strLength + 5) * sizeof(wchar_t);

    wchar_t * const tmpStr = reinterpret_cast<wchar_t*>(memory::StackAlloc(bufferSize));
    swprintf_s(tmpStr, bufferSize, L"%s.dll", name);

    return reinterpret_cast<LibraryHandle>(LoadLibraryW(tmpStr));
}

void* LibraryLoader::GetFunctionAddress(LibraryHandle lib, const char *functionName)
{
    return GetProcAddress(reinterpret_cast<HMODULE>(lib), functionName);
}