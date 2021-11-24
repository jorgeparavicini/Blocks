// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: WindowClass.h

#pragma once
#include <memory>

#include "BlocksEngine/Main/WindowOptions.h"

namespace BlocksEngine::Internal
{
    class WindowClass;
}

class BlocksEngine::Internal::WindowClass
{
public:
    explicit WindowClass(std::wstring name,
                         std::unique_ptr<WindowOptions> options,
                         WNDPROC windowProc);
    ~WindowClass();
    WindowClass(const WindowClass&) = delete;
    WindowClass& operator=(const WindowClass&) = delete;
    WindowClass(WindowClass&&) = delete;
    WindowClass& operator=(WindowClass&&) = delete;

    [[nodiscard]] const std::wstring& Name() const noexcept;
    [[nodiscard]] HINSTANCE Instance() const noexcept;
    [[nodiscard]] const WindowOptions& Options() const noexcept;


private:
    HINSTANCE hInst_;
    const std::wstring name_;
    std::unique_ptr<WindowOptions> options_;
};
