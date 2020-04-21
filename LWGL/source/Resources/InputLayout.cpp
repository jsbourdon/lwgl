#include <pch.h>
#include "Resources/InputLayout.h"

using namespace lwgl;
using namespace resources;

InputLayout::~InputLayout()
{
    SAFE_RELEASE(m_pLayout);
}
