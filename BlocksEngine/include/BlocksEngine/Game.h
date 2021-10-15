// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Game.h

#pragma once

#include "Camera.h"
#include "Window.h"
#include "WindowOptions.h"

namespace BlocksEngine
{
    class Game;
}

class BlocksEngine::Game
{
public:
    explicit Game(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>());

    [[nodiscard]] int MainLoop() const;
    [[nodiscard]] Window& GetWindow() const noexcept;
    [[nodiscard]] const Graphics& GetGraphics() const noexcept;
    [[nodiscard]] Camera& GetCamera() const noexcept;
    [[nodiscard]] bool HasCamera() const noexcept;
    [[nodiscard]] const Keyboard& Keyboard() const noexcept;
    [[nodiscard]] const Mouse& Mouse() const noexcept;


    static void Exit() noexcept;
    void ForceExit() noexcept;
    void SetActiveCamera(Camera& camera);

    Actor& AddActor();

private:
    int actorCount_{0};
    bool shutdownForced_{false};
    std::unique_ptr<Window> pWindow_{};
    void Tick() const;
    void Update() const;
    void Render() const;
    static std::optional<int> ProcessApplicationMessages() noexcept;

    Camera* camera_{nullptr};
    std::vector<std::unique_ptr<Actor>> pActors_{};
};
