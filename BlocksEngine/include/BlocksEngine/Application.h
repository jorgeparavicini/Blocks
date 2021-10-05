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

#include "Game.h"
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
    explicit Application(std::unique_ptr<WindowOptions> options);
    [[nodiscard]] int MainLoop() const;
    static void Exit() noexcept;
    void ForceExit() noexcept;

private:
    bool shutdownForced_{false};
    std::unique_ptr<Window> pWindow_;
    std::shared_ptr<Game> pGame_;


    void Tick() const;
    void Update();
    static std::optional<int> ProcessApplicationMessages() noexcept;
};
