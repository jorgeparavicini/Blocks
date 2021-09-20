#include "pch.h"
#include "GraphicsException.h"

const char* BlocksEngine::GraphicsException::GetType() const noexcept
{
    return "Window Exception [No Graphics]";
}
