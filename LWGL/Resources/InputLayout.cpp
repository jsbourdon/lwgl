#include <pch.h>
#include "InputLayout.h"

using namespace lwgl;
using namespace resources;

InputLayout::~InputLayout()
{
    SAFE_RELEASE(m_pLayout);
}
