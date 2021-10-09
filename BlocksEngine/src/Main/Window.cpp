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

    RECT rect;
    GetWindowRect(hWnd_, &rect);

    pGraphics_ = std::make_unique<Graphics>(hWnd_, rect.right - rect.left, rect.bottom - rect.top);

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
                                              const LPARAM lParam)
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
                                              const LPARAM lParam)
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
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

const BlocksEngine::Graphics& BlocksEngine::Window::Gfx() const
{
    return *pGraphics_;
}

void BlocksEngine::Window::Clear() const
{
    pGraphics_->Clear();
}

void BlocksEngine::Window::Present() const
{
    pGraphics_->Present();
}

void BlocksEngine::Window::OnWindowSizeChanged(const int width, const int height) const
{
    pGraphics_->OnWindowSizeChanged(width, height);
}

void BlocksEngine::Window::SetMinWindowSize(const int minWidth, const int minHeight)
{
    minWidth_ = std::max(minWidth, 1);
    minHeight_ = std::max(minHeight, 1);
}

// TODO: Find a good signal library (maybe Sevi?)
void BlocksEngine::Window::SetOnSuspending(std::function<void()> function)
{
    onSuspending_ = std::move(function);
}

/**
 * Handle all Window Messages
 */
// ReSharper disable once CppParameterMayBeConst
LRESULT BlocksEngine::Window::WindowProc(HWND hWnd, const UINT uMsg, const WPARAM wParam,
                                         const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!isMinimized_)
            {
                isMinimized_ = true;
                if (!isSuspended_ && onSuspending_)
                {
                    onSuspending_();
                }
                isSuspended_ = true;
            }
        }
        else if (isMinimized_)
        {
            isMinimized_ = false;
            if (isSuspended_ && onResuming_)
            {
                onResuming_();
            }
            isSuspended_ = false;
        }
        else if (isResizing_)
        {
            pGraphics_->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }

    case WM_ENTERSIZEMOVE:
        isResizing_ = true;
        break;

    case WM_EXITSIZEMOVE:
        isResizing_ = false;
        RECT rc;
        GetClientRect(hWnd_, &rc);

        pGraphics_->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        break;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            const auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = minWidth_;
            info->ptMinTrackSize.y = minHeight_;
        }
        break;

    case WM_SYSKEYDOWN:
        // Alt + Enter
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            if (isFullscreen_)
            {
                SetWindowLongPtr(hWnd_, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd_, GWL_EXSTYLE, 0);

                ShowWindow(hWnd, SW_SHOWNORMAL);
                SetWindowPos(hWnd, HWND_TOP, 0, 0, defaultWidth_, defaultHeight_,
                             SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd_, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hWnd_, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
                ShowWindow(hWnd_, SW_SHOWMAXIMIZED);
            }

            isFullscreen_ = !isFullscreen_;
        }


    default: break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
