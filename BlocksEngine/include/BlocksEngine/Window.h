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
#include "BlocksEngine/WindowClass.h"
#include "BlocksEngine/WindowOptions.h"

namespace BlocksEngine
{
    class Window;
}

class BlocksEngine::Window
{
public:
    explicit Window(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>(),
                    std::wstring name = L"Default Window",
                    int x = CW_USEDEFAULT,
                    int y = CW_USEDEFAULT,
                    int width = CW_USEDEFAULT,
                    int height = CW_USEDEFAULT
    );

    ~Window() = default;
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] std::optional<int> ProcessMessages() const noexcept;
    [[nodiscard]] const Graphics& Gfx() const;

    void Clear() const;
    void Present() const;
    void OnWindowSizeChanged(int width, int height) const;
    void SetMinWindowSize(int minWidth, int minHeight);
    void SetOnSuspending(std::function<void()> function);

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
};
