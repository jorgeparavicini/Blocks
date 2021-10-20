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
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit Game(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>());

    /**
     * \brief Starts the blocking Main Loop. 
     * \return Return code if something went wrong. A return value of 0 indicates no failure.
     */
    [[nodiscard]] int MainLoop() const;
    [[nodiscard]] Window& Window() const noexcept;
    [[nodiscard]] const Graphics& Graphics() const noexcept;
    [[nodiscard]] Camera& MainCamera() const noexcept;
    [[nodiscard]] bool HasCamera() const noexcept;
    [[nodiscard]] const Keyboard& Keyboard() const noexcept;
    [[nodiscard]] const Mouse& Mouse() const noexcept;


    static void Exit() noexcept;
    void ForceExit() noexcept;
    void SetActiveCamera(Camera& camera);

    Actor& AddActor();

private:
    int totalActorCount_{0};
    bool shutdownForced_{false};
    std::unique_ptr<BlocksEngine::Window> pWindow_{};
    void Tick() const;
    void Update() const;
    void Render() const;
    static std::optional<int> ProcessApplicationMessages() noexcept;

    Camera* camera_{nullptr};
    std::vector<std::unique_ptr<Actor>> pActors_{};
};
