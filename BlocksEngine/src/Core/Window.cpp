#include "BlocksEngine/pch.h"
#include "BlocksEngine/Window.h"

#include <unordered_map>

#include "BlocksEngine/WindowException.h"

BlocksEngine::Window::Window(const std::wstring name,
                             const int x,
                             const int y,
                             const int width,
                             const int height,
                             std::unique_ptr<WindowOptions> options)
    : windowClass_{name, std::move(options), WindowProcSetup},
      hWnd_{
          CreateWindowEx(windowClass_.Options().dwExStyle,
                         windowClass_.Name().c_str(),
                         name.c_str(),
                         windowClass_.Options().dwStyle,
                         x,
                         y,
                         width,
                         height,
                         nullptr,
                         nullptr,
                         windowClass_.Instance(),
                         this)
      }
{
    if (hWnd_ == nullptr)
    {
        throw WindowException(__LINE__, __FILE__, static_cast<HRESULT>(GetLastError()));
    }

    ShowWindow(hWnd_, SW_SHOWDEFAULT);
}

BlocksEngine::Window::~Window()
{
    DestroyWindow(hWnd_);
}

LRESULT BlocksEngine::Window::WindowProcSetup(HWND hWnd, const UINT uMsg, const WPARAM wParam,
                                              const LPARAM lParam) noexcept
{
    if (uMsg == WM_NCCREATE)
    {
        const auto* const pCreate = reinterpret_cast<const CREATESTRUCT*>(lParam);
        const auto pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WindowProcRelay));
        return pWnd->WindowProc(hWnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT BlocksEngine::Window::WindowProcRelay(HWND hWnd, const UINT uMsg, const WPARAM wParam,
                                              const LPARAM lParam) noexcept
{
    const auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->WindowProc(hWnd, uMsg, wParam, lParam);
}


std::optional<int> BlocksEngine::Window::ProcessMessages() const noexcept
{
    MSG msg;

    while (PeekMessage(&msg, hWnd_, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

LRESULT BlocksEngine::Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
