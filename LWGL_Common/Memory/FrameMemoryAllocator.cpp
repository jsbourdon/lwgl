#include <pch.h>
#include "FrameMemoryAllocator.h"

using namespace lwgl::memory;

FrameMemoryAllocator::FrameMemoryAllocator(size_t poolByteSize)
{
    Init(poolByteSize);
}

FrameMemoryAllocator::~FrameMemoryAllocator()
{
    if (m_MemoryPool)
    {
        FreeAlignedAlloc(m_MemoryPool);
    }
}

void FrameMemoryAllocator::Init(size_t poolByteSize)
{
    m_PoolByteSize = Align(poolByteSize, s_DefaultAlignment);
    m_MemoryPool = static_cast<uint8_t*>(AlignedAlloc(m_PoolByteSize, s_DefaultAlignment));
}

void* FrameMemoryAllocator::Allocate(size_t byteSize, size_t alignment)
{
    LWGL_ASSERT(alignment <= s_DefaultAlignment, "16 is maximum supported alignment for this allocator");

    size_t allocByteSize = Align(byteSize, alignment);
    size_t alignedOffset = Align(m_NextByte, alignment);
    m_NextByte = alignedOffset + allocByteSize;

    LWGL_ASSERT(m_NextByte <= m_PoolByteSize, "Over allocation for this frame");

    return m_MemoryPool + alignedOffset;
}

void FrameMemoryAllocator::Reset(MemoryBookmark bookmark)
{
    m_NextByte = bookmark;
}

void FrameMemoryAllocator::Reset()
{
    m_NextByte = 0;
}
