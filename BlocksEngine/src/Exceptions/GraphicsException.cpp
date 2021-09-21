#include "BlocksEngine/pch.h"
#include "BlocksEngine/GraphicsException.h"

const char* BlocksEngine::GraphicsException::GetType() const noexcept
{
    return "Window Exception [No Graphics]";
}
