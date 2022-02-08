#include "BlocksEngine/pch.h"
#include "BlocksEngine/Exceptions/EngineException.h"

#include <sstream>

BlocksEngine::EngineException::EngineException(const int line, const std::string file, std::string msg) noexcept
    : Exception{line, file},
      msg_{std::move(msg)}
{
}

const char* BlocksEngine::EngineException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n' << "[Description] " << msg_ << std::flush;

    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

const char* BlocksEngine::EngineException::GetType() const noexcept
{
    return "Engine Exception";
}
