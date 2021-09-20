// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Window.h

#pragma once
#include <optional>
#include <unordered_map>

#include "WindowClass.h"
#include "WindowOptions.h"

namespace BlocksEngine
{
    class Window;
}

class BlocksEngine::Window
{
public:
    explicit Window(std::wstring name = L"Default Window",
                    int x = CW_USEDEFAULT,
                    int y = CW_USEDEFAULT,
                    int width = CW_USEDEFAULT,
                    int height = CW_USEDEFAULT,
                    std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>());

    // Do not allow the window to be copied
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    [[nodiscard]] std::optional<int> ProcessMessages() const noexcept;

protected:
    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

private:
    static LRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK WindowProcRelay(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
    static constexpr const wchar_t* ClassName = L"Blocks Engine Window";

    Internal::WindowClass windowClass_;
    HWND hWnd_;
};
