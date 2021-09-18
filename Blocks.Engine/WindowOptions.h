// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: WindowOptions.h

#pragma once
#include <Windows.h>

namespace BlocksEngine
{
    struct WindowOptions;
}

struct BlocksEngine::WindowOptions
{
    UINT style{};
    int cbClsExtra{};
    int cbWndExtra{};
    HICON hIcon{};
    HCURSOR hCursor{};
    HBRUSH hbrBackground{};
    LPCWSTR lpszMenuName{};
    HICON hIconSm{};
    DWORD dwExStyle{};
    DWORD dwStyle{};
};
