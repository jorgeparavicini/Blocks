#include "BlocksEngine/pch.h"
#include "BlocksEngine/Exceptions/BufferException.h"

BlocksEngine::BufferException::BufferException(const int line, const std::string file, const std::string msg) noexcept
    : EngineException{line, file, msg}
{
}

const char* BlocksEngine::BufferException::GetType() const noexcept
{
    return "Buffer Exception";
}
