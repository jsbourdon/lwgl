#pragma once

#include <type_traits>

#ifndef ARRAYSIZE
    #define ARRAYSIZE(A) std::extent<decltype(A)>::value
#endif 

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }
#endif

#ifdef _DEBUG
    #define CHECK_HRESULT_RETURN_VALUE(x, v)                                                                        \
        hr = (x);                                                                                                   \
        if (FAILED(hr))                                                                                             \
        {                                                                                                           \
            char tmp[1024];                                                                                         \
            sprintf_s(tmp, ARRAYSIZE(tmp), "Statement %s failed with HRESULT %lX", #x, hr);                         \
            int selection = MessageBoxA(NULL, tmp, "COM Call Failed", MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);   \
            if (selection == IDCANCEL) DebugBreak();                                                                \
            return v;                                                                                               \
        }

    #define CHECK_HRESULT_BOOL(x) CHECK_HRESULT_RETURN_VALUE(x, false);
    #define CHECK_HRESULT_PTR(x) CHECK_HRESULT_RETURN_VALUE(x, nullptr);
#else
    #define CHECK_RESULT(x) if (FAILED(x)) { return false; }
#endif
