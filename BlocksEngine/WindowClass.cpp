#include "pch.h"
#include "WindowClass.h"

#include "WindowOptions.h"

BlocksEngine::Internal::WindowClass::WindowClass(const std::wstring name,
                                                 std::unique_ptr<WindowOptions> options,
                                                 WNDPROC windowProc)
    : hInst_{GetModuleHandle(nullptr)},
      name_{name},
      options_{std::move(options)}
{
    if (options_ == nullptr)
    {
        options_ = std::make_unique<WindowOptions>();
    }

    WNDCLASSEX wc;
    wc.cbSize = sizeof wc;
    wc.style = Options().style;
    wc.lpfnWndProc = windowProc;
    wc.cbClsExtra = Options().cbClsExtra;
    wc.cbWndExtra = Options().cbWndExtra;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = Options().hIcon;
    wc.hCursor = Options().hCursor;
    wc.hbrBackground = Options().hbrBackground;
    wc.lpszMenuName = Options().lpszMenuName;
    wc.lpszClassName = name.c_str();
    wc.hIconSm = Options().hIcon;

    RegisterClassEx(&wc);
}

BlocksEngine::Internal::WindowClass::~WindowClass()
{
    UnregisterClass(name_.c_str(), Instance());
}


const std::wstring& BlocksEngine::Internal::WindowClass::Name() const noexcept
{
    return name_;
}

HINSTANCE BlocksEngine::Internal::WindowClass::Instance() const noexcept
{
    return hInst_;
}

const BlocksEngine::WindowOptions& BlocksEngine::Internal::WindowClass::Options() const noexcept
{
    return *options_;
}
