#include <pch.h>
#include "GfxCommandQueue.h"

using namespace lwgl;
using namespace lwgl::command;

void GfxCommandQueue::ExecuteCommandLists(uint32_t count, const GfxCommandList **pLists)
{
    // #todo Implement
}

void GfxCommandQueue::SignalFence(FenceHandle hdl, uint64_t value)
{
    // #todo Implement
}

void GfxCommandQueue::WaitForFence(FenceHandle hdl, uint64_t value)
{
    // #todo Implement
}
