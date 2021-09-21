//
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Application.h
//

#pragma once
#include <memory>

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
    Application();
    [[nodiscard]] bool GetCloseOnWindowsDestroyed() const noexcept;
    void SetCloseOnWindowsDestroyed(bool value) noexcept;

    int MainLoop();
    void RequestShutdown() noexcept;
    void RequestImmediateShutdown() noexcept;

private:
    bool shutdownRequested_{false};
    bool shutdownForced_{false};
    bool closeOnWindowsDestroyed_{false};
    std::unique_ptr<Window> window_;
    int ForceShutdown();
};
