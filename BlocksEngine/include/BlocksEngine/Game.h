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

namespace BlocksEngine
{
    class Application;
    class Game;
}

class BlocksEngine::Game
{
public:
    explicit Game(const Application& application);

    void SetActiveCamera(Camera& camera);
    [[nodiscard]] Camera& GetCamera() const noexcept;
    [[nodiscard]] bool HasCamera() const noexcept;
    [[nodiscard]] const Application& GetApplication() const noexcept;
    [[nodiscard]] const Graphics& GetGraphics() const noexcept;

    Actor& AddActor();

    void Update() const;
    void Render() const;

private:
    const Application& application_;
    Camera* camera_{nullptr};
    std::vector<std::unique_ptr<Actor>> pActors_{};
};
