﻿//
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Application.h
//

#pragma once
#include <vector>

#include "Window.h"

// ReSharper disable CppInconsistentNaming

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
inline __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
inline __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// ReSharper restore CppInconsistentNaming

namespace BlocksEngine
{
    class Application;
}

class BlocksEngine::Application
{
public:
    [[nodiscard]] bool GetCloseOnWindowsDestroyed() const noexcept;
    void SetOnWindowsDestroyed(bool value) noexcept;
    void RegisterWindow(std::unique_ptr<Window> window) noexcept;

private:
    bool closeOnWindowsDestroyed_{false};
    std::vector<std::unique_ptr<Window>> windows_{};
};
