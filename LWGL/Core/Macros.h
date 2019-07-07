#pragma once

#include <type_traits>

#ifndef ARRAYSIZE
    #define ARRAYSIZE(A) std::extent<decltype(A)>::value
#endif 

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = nullptr; } }
#endif

#define SAFE_ADDREF(p) { if (p) { p->AddRef(); } }

#ifdef _DEBUG
    #define CHECK_HRESULT_RETURN_VALUE_INTERNAL(hr, x, v)                                                               \
        {                                                                                                               \
            HRESULT hr = (x);                                                                                           \
            if (FAILED(hr))                                                                                             \
            {                                                                                                           \
                char tmp[1024];                                                                                         \
                sprintf_s(tmp, ARRAYSIZE(tmp), "Statement %s failed with HRESULT %lX", #x, hr);                         \
                int selection = MessageBoxA(NULL, tmp, "COM Call Failed", MB_OKCANCEL | MB_ICONERROR | MB_TASKMODAL);   \
                if (selection == IDCANCEL) DebugBreak();                                                                \
                return v;                                                                                               \
            }                                                                                                           \
        }
#else
    #define CHECK_HRESULT_RETURN_VALUE_INTERNAL(hr, x, v)                                                               \
        {                                                                                                               \
            HRESULT hr = (x);                                                                                           \
            if (FAILED(hr))                                                                                             \
            {                                                                                                           \
                return v;                                                                                               \
            }                                                                                                           \
        }
#endif

#define CHECK_HRESULT(x) CHECK_HRESULT_RETURN_VALUE_INTERNAL(hr_##__FILE__##__LINE__, x, hr_##__FILE__##__LINE__)
#define CHECK_HRESULT_RETURN_VALUE(x, v) CHECK_HRESULT_RETURN_VALUE_INTERNAL(hr_##__FILE__##__LINE__, x, v)
