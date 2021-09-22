#include "BlocksEngine/pch.h"
#include "BlocksEngine/DxgiInfoException.h"

#include <sstream>

BlocksEngine::DxgiInfoException::DxgiInfoException(const int line, const char* file,
                                                   const std::vector<std::string> infoMsgs) noexcept
    : Exception{line, file}
{
    // Separate info messages with newline
    for (const auto& m : infoMsgs)
    {
        info_ += m;
        info_.push_back('\n');
    }

    // Remove final newline if it exists.
    if (!info_.empty())
    {
        info_.pop_back();
    }
}

const char* BlocksEngine::DxgiInfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << "\n[Error Info]\n" << GetErrorInfo() << '\n' << std::endl;
    oss << GetOriginString();
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

const char* BlocksEngine::DxgiInfoException::GetType() const noexcept
{
    return "Dxgi Info Exception";
}

std::string BlocksEngine::DxgiInfoException::GetErrorInfo() const noexcept
{
    return info_;
}
