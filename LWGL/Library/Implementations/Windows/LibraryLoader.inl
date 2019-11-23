#pragma once

#include <stdio.h>
#include "LibraryLoader.h"
#include "LWGL_Common/Memory/Memory.h"

using namespace lwgl;
using namespace lwgl::external;

static wchar_t g_LibraryFileName[32];

LibraryHandle LibraryLoader::LoadDynamicLibrary(const wchar_t *name)
{
    const size_t strLength = wcslen(name);
    const size_t bufferSize = (strLength + 5) * sizeof(wchar_t);

    swprintf_s(g_LibraryFileName, ARRAY_SIZE(g_LibraryFileName), L"%s.dll", name);
    LibraryHandle libHdl = reinterpret_cast<LibraryHandle>(LoadLibraryW(g_LibraryFileName));

//     wchar_t * const tmpStr = reinterpret_cast<wchar_t*>(memory::StackAlloc(bufferSize));
//     swprintf_s(tmpStr, bufferSize, L"%s.dll", name);

//     LibraryHandle libHdl = reinterpret_cast<LibraryHandle>(LoadLibraryW(tmpStr));
//     InitLibFnctPtr initFnctPtr = reinterpret_cast<InitLibFnctPtr>(GetFunctionAddress(libHdl, "InitLib"));
//     bool initSuccess = (*initFnctPtr)();

//    return initSuccess ? libHdl : Handle_NULL;

    return libHdl;
}

void LibraryLoader::UnloadDynamicLibrary(LibraryHandle handle)
{
    FreeLibrary(reinterpret_cast<HMODULE>(handle));
}

void* LibraryLoader::GetFunctionAddress(LibraryHandle lib, const char *functionName)
{
    return GetProcAddress(reinterpret_cast<HMODULE>(lib), functionName);
}