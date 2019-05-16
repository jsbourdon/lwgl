#include <pch.h>
#include "Buffer.h"

using namespace lwgl;
using namespace resources;

Buffer::Buffer()
    : base()
    , m_pD3DBuffer(nullptr)
{

}

Buffer::~Buffer()
{
    SAFE_RELEASE(m_pD3DBuffer);
}
