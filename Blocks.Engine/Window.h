// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Window.h

#pragma once

namespace BlocksEngine
{
    class Window;
}

class BlocksEngine::Window
{
public:
    explicit Window(int width = CW_USEDEFAULT, int height = CW_USEDEFAULT) noexcept;
    explicit Window(std::wstring name, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT) noexcept;

private:
    static HRESULT CALLBACK WindowProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
    static HRESULT CALLBACK WindowProcRelay(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

protected:
    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
};
