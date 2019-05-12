#pragma once

#include <type_traits>

#ifndef ARRAYSIZE
    #define ARRAYSIZE(A) std::extent<decltype(A)>::value
#endif 

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }
#endif

#ifdef _DEBUG
    #define CHECK_HRESULT_RETURN_VALUE(x, v)                                                                \
        hr = (x);                                                                                           \
        if (FAILED(hr))                                                                                     \
        {                                                                                                   \
            char tmp[1024];                                                                                 \
            sprintf_s(tmp, ARRAYSIZE(tmp), "Statement %s failed with HRESULT %ld", #x, hr);                \
            MessageBoxA(NULL, tmp, "COM Call Failed", MB_CANCELTRYCONTINUE | MB_ICONERROR | MB_TASKMODAL);  \
            return v;                                                                                   \
        }

    #define CHECK_HRESULT_BOOL(x) CHECK_HRESULT_RETURN_VALUE(x, false);
    #define CHECK_HRESULT_PTR(x) CHECK_HRESULT_RETURN_VALUE(x, nullptr);
#else
    #define CHECK_RESULT(x) if (FAILED(x)) { return false; }
#endif
