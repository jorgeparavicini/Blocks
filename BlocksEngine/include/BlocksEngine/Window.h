// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Window.h

#pragma once
#include <functional>
#include <optional>
#include <unordered_map>

#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "BlocksEngine/WindowClass.h"
#include "BlocksEngine/WindowOptions.h"

namespace BlocksEngine
{
    class Window;
}

class BlocksEngine::Window
{
public:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    /**
     * \brief Creates a new window which contains the graphic context attached to it.
     * \param options The window options to pass to the system window creation. See https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa for more info.
     * \param name The title of the window.
     * \param x The initial x position of the window from the left of the screen.
     * \param y The initial y position of the window from the top of the screen.
     * \param width The initial width of the window.
     * \param height The initial height of the window.
     */
    explicit Window(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>(),
                    std::wstring name = L"Default Window",
                    int x = CW_USEDEFAULT,
                    int y = CW_USEDEFAULT,
                    int width = CW_USEDEFAULT,
                    int height = CW_USEDEFAULT
    );


    /**
     * \brief Disable the duplication of a window.
     */
    Window(const Window&) = delete;

    /**
     * \brief Disable the duplication of a window.
     */
    Window& operator=(const Window&) = delete;

    /**
     * \brief Disable the duplication of a window.
     */
    Window(Window&&) = delete;

    /**
     * \brief Disable the duplication of a window.
     */
    Window& operator=(Window&&) = delete;

    ~Window() = default;

    //------------------------------------------------------------------------------
    // Properties
    //------------------------------------------------------------------------------


    /**
     * \brief Get the graphics context attached to this window.
     * \return The reference to the graphics context.
     */
    [[nodiscard]] const Graphics& Gfx() const noexcept;

    /**
     * \brief Get the mouse input handler.
     * \return The mouse input handler
     */
    [[nodiscard]] const Mouse& Mouse() const noexcept;

    /**
     * \brief Get the keyboard input handler.
     * \return The keyboard input handler
     */
    [[nodiscard]] const Keyboard& Keyboard() const noexcept;


    /**
     * \brief Process all system messages connected to this window
     * \return An optional value containing the error code if the window has been closed.
     * If the window is still alive the return value will be an empty optional.
     */
    [[nodiscard]] std::optional<int> ProcessMessages() const noexcept;

    [[nodiscard]] HWND HWnd() const noexcept;


    /**
     * \brief Clears the back buffer with the default color.
     */
    void Clear() const;

    /**
     * \brief Presents the rendering calls to the display and swaps buffers.
     */
    void Present() const;

    /**
     * \brief Set the minimum window size. The window will not be able to be resized below this size.
     */
    void SetMinWindowSize(int minWidth, int minHeight);


    //------------------------------------------------------------------------------
    // Signals
    //------------------------------------------------------------------------------

    // TODO: Convert to signal
    void SetOnSuspending(std::function<void()> function);

    /**
     * \brief Add a listener to be notified when the window has been resized.
     * \param slot The listener slot to be added.
     * \return The signal connection that can be used to stop listening to the signal.
     */
    [[nodiscard]] boost::signals2::connection AddSignalWindowResized(
        const Graphics::WindowResizedSignal::slot_type& slot) const noexcept;


protected:
    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WindowProcRelay(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static constexpr const wchar_t* ClassName = L"Blocks Engine Window";

    Internal::WindowClass windowClass_;
    HWND hWnd_{};
    std::unique_ptr<Graphics> pGraphics_{};

    int minWidth_{320};
    int minHeight_{200};
    int defaultWidth_{800};
    int defaultHeight_{600};

    bool isResizing_{false};
    bool isSuspended_{false};
    bool isMinimized_{false};
    bool isFullscreen_{false};

    // Events
    std::function<void()> onSuspending_;
    std::function<void()> onResuming_;

    std::unique_ptr<BlocksEngine::Mouse> pMouse_{std::make_unique<BlocksEngine::Mouse>()};
    std::unique_ptr<BlocksEngine::Keyboard> pKeyboard_{std::make_unique<BlocksEngine::Keyboard>()};
};
