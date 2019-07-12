#include <pch.h>
#include "TextureArray.h"

using namespace lwgl;
using namespace resources;

TextureArray::~TextureArray()
{
    // Starting at one since the base class already released the first
    for (size_t i = 1; i < m_ArraySize; ++i)
    {
        SAFE_RELEASE(m_pRTVs[i]);
    }

    delete[] m_pRTVs;
}
