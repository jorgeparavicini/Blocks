#include "pch.h"
#include "WindowException.h"

#include <sstream>

BlocksEngine::WindowException::WindowException(const int line, const char* file, const HRESULT hr) noexcept
    : Exception{line, file},
      hr_{hr}
{
}

const char* BlocksEngine::WindowException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << "[Error Code] " << GetErrorCode() << '\n'
        << "[Description] " << GetErrorDescription() << '\n'
        << GetOriginString() << std::flush;

    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

const char* BlocksEngine::WindowException::GetType() const noexcept
{
    return "Window Exception";
}

HRESULT BlocksEngine::WindowException::GetErrorCode() const noexcept
{
    return hr_;
}

std::string BlocksEngine::WindowException::GetErrorDescription() const noexcept
{
    return TranslateErrorCode(hr_);
}
