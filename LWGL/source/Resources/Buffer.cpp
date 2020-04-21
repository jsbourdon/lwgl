#include <pch.h>
#include "Resources/Buffer.h"
#include "Descriptors/BufferDescriptor.h"

using namespace lwgl;
using namespace resources;
using namespace descriptors;

Buffer::Buffer()
    : base()
    , m_pD3DBuffer(nullptr)
    , m_pD3DBufferSRV(nullptr)
    , m_BufferType(BufferType::EnumCount)
{

}

Buffer::~Buffer()
{
    SAFE_RELEASE(m_pD3DBufferSRV);
    SAFE_RELEASE(m_pD3DBuffer);
}
