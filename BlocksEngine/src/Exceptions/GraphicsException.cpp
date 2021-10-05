#include "BlocksEngine/pch.h"
#include "BlocksEngine/GraphicsException.h"

#include <sstream>

BlocksEngine::GraphicsException::GraphicsException(const int line, const char* file, const HRESULT hr,
                                                   const std::vector<std::string> infoMsgs) noexcept
    : DxgiInfoException{line, file, infoMsgs},
      hr_{hr}
{
}

const char* BlocksEngine::GraphicsException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << '\n'
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << "(" << static_cast<unsigned long>(GetErrorCode()) << ")" << '\n'
        << "[Error Description] " << GetErrorDescription() << std::endl;

    if (!GetErrorInfo().empty())
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << '\n' << std::endl;
    }

    oss << GetOriginString();
    whatBuffer_ = oss.str();
    return whatBuffer_.c_str();
}

HRESULT BlocksEngine::GraphicsException::GetErrorCode() const noexcept
{
    return hr_;
}

std::string BlocksEngine::GraphicsException::GetErrorDescription() const noexcept
{
    return TranslateErrorCode(hr_);
}

const char* BlocksEngine::GraphicsException::GetType() const noexcept
{
    return "Graphics Exception";
}
