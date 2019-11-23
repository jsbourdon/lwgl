// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return TRUE;
}

extern "C"
{
    EXPORT_SYMBOL
    bool InitLib()
    {
        return true;
    }
}