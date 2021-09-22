#include "BlocksEngine/pch.h"
#include "BlocksEngine/Window.h"

#include "BlocksEngine/WindowException.h"

BlocksEngine::Window::Window(std::unique_ptr<WindowOptions> options,
                             const std::wstring name,
                             const int x,
                             const int y,
                             const int width,
                             const int height)
    : windowClass_{name, std::move(options), WindowProcSetup}
{
    hWnd_ = CreateWindowEx(windowClass_.Options().dwExStyle,
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
                           this);

    if (hWnd_ == nullptr)
    {
        throw WindowException(__LINE__, __FILE__, static_cast<HRESULT>(GetLastError()));
    }

    ShowWindow(hWnd_, SW_SHOWDEFAULT);
}


/**
 * Initializes the relay WindowProc function
 *
 * First it retrieves the custom Window instance that created the system window responsible for sending this message.
 * It then stores this Window reference in the UserData for the hWnd and points the WindowProc to WindowProcRelay
 */
// ReSharper disable once CppParameterMayBeConst
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

/**
 * Gets the Window reference associated with the calling hWnd and calls the WindowProc for that instance.
 */
// ReSharper disable once CppParameterMayBeConst
LRESULT BlocksEngine::Window::WindowProcRelay(HWND hWnd, const UINT uMsg, const WPARAM wParam,
                                              const LPARAM lParam) noexcept
{
    const auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->WindowProc(hWnd, uMsg, wParam, lParam);
}

/**
 * Handle all messages on the message stack and return if there are no more messages
 * If something is returned (not an empty optional) then the application should stop with the return code inside the optional.
 */
std::optional<int> BlocksEngine::Window::ProcessMessages() const noexcept
{
    MSG msg;

    while (PeekMessage(&msg, hWnd_, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return static_cast<int>(msg.wParam);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

/**
 * Handle all Window Messages
 */
// ReSharper disable once CppParameterMayBeConst
LRESULT BlocksEngine::Window::WindowProc(HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam) noexcept
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default: break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
