#include "BlocksEngine/pch.h"
#include "BlocksEngine/Window.h"

#include <Windows.h>
#include <wingdi.h>

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

const BlocksEngine::Graphics& BlocksEngine::Window::Gfx() const noexcept
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

void BlocksEngine::Window::SetMinWindowSize(const int minWidth, const int minHeight)
{
    minWidth_ = std::max(minWidth, 1);
    minHeight_ = std::max(minHeight, 1);
}

// TODO: Find a good signal library (maybe Sevi?) -> boost::signals2
void BlocksEngine::Window::SetOnSuspending(std::function<void()> function)
{
    onSuspending_ = std::move(function);
}

boost::signals2::connection BlocksEngine::Window::AddSignalWindowResized(
    const Graphics::WindowResizedSignal::slot_type& slot) const noexcept
{
    return pGraphics_->AddSignalWindowResized(slot);
}

const BlocksEngine::Mouse& BlocksEngine::Window::Mouse() const noexcept
{
    return *pMouse_;
}

const BlocksEngine::Keyboard& BlocksEngine::Window::Keyboard() const noexcept
{
    return *pKeyboard_;
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
        /************* Mouse Messages **************/

    case WM_MOUSEMOVE:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            if (x >= 0 && x < Gfx().Width() && y >= 0 && y < Gfx().Height())
            {
                pMouse_->OnMouseMove(x, y);
                if (!pMouse_->IsInWindow())
                {
                    SetCapture(hWnd);
                    pMouse_->OnMouseEnter();
                }
            }
            else
            {
                if (wParam & (MK_LBUTTON | MK_RBUTTON))
                {
                    pMouse_->OnMouseMove(x, y);
                }
                else
                {
                    ReleaseCapture();
                    pMouse_->OnMouseLeave();
                }
            }
            break;
        }

    case WM_LBUTTONDOWN:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            pMouse_->OnLeftPressed(x, y);
            SetForegroundWindow(hWnd);
            break;
        }

    case WM_LBUTTONUP:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            pMouse_->OnLeftReleased(x, y);

            if (x < 0 || x >= Gfx().Width() || y < 0 || y >= Gfx().Height())
            {
                ReleaseCapture();
                pMouse_->OnMouseLeave();
            }
            break;
        }

    case WM_RBUTTONDOWN:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            pMouse_->OnRightPressed(x, y);
            break;
        }

    case WM_RBUTTONUP:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            pMouse_->OnRightReleased(x, y);

            if (x < 0 || x >= Gfx().Width() || y < 0 || y >= Gfx().Height())
            {
                ReleaseCapture();
                pMouse_->OnMouseLeave();
            }
            break;
        }

    case WM_MOUSEWHEEL:
        {
            const auto [x, y] = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            pMouse_->OnWheelDelta(x, y, delta);
            break;
        }

        /*********** End Mouse Messages ************/

        /************ Keyboard Messages ************/

    case WM_KEYDOWN:
        if (!(lParam & 0x40000000) || pKeyboard_->IsAutorepeatEnabled())
        {
            pKeyboard_->OnKeyPressed(static_cast<unsigned char>(wParam));
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
        else
        {
            if (!(lParam & 0x40000000) || pKeyboard_->IsAutorepeatEnabled())
            {
                pKeyboard_->OnKeyPressed(static_cast<unsigned char>(wParam));
            }
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        pKeyboard_->OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        pKeyboard_->OnChar(static_cast<char>(wParam));
        break;

        /********** End Keyboard Messages **********/

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
            //pGraphics_->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
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


    default: break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
