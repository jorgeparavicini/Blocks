#include "pch.h"
#include "Exception.h"

#include <sstream>

BlocksEngine::Exception::Exception(const int line, const std::string file) noexcept
    : line_{line}, file_{file}
{
}

const char* BlocksEngine::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n' << GetOriginString() << std::flush;
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

const char* BlocksEngine::Exception::GetType() const noexcept
{
    return "BlocksEngine exception";
}

int BlocksEngine::Exception::GetLine() const noexcept
{
    return line_;
}

const std::string& BlocksEngine::Exception::GetFile() const noexcept
{
    return file_;
}

std::string BlocksEngine::Exception::GetOriginString() const noexcept
{
    std::ostringstream oss;
    oss << "[File] " << file_ << '\n' << "[Line] " << line_ << std::flush;
    return oss.str();
}

std::string BlocksEngine::Exception::TranslateErrorCode(const HRESULT hr) noexcept
{
    char* pMsgBuf{};

    // 0 string length returned indicates a failure
    if (FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr) == 0)
    {
        return "Unidentified error code";
    }

    // Copy error string from windows-allocated buffer to std::string
    std::string errorString{pMsgBuf};

    // Free system buffer
    LocalFree(pMsgBuf);
    return errorString;
}
