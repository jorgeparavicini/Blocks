// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Component.h

#pragma once

#include <memory>

namespace BlocksEngine
{
    class Transform;
    class Actor;
    class Game;

    class Component;
}

class BlocksEngine::Component
{
public:
    /**
     * Subclasses need to put GetActor& as first parameter.
     */
    explicit Component(std::weak_ptr<Actor> actor);

    virtual ~Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(const Component&&) = delete;
    Component& operator=(const Component&&) = delete;

    [[nodiscard]] std::shared_ptr<Game> GetGame() const noexcept;
    /**
     * Gets the actor this component is attached to.
     */
    [[nodiscard]] std::shared_ptr<Actor> GetActor() const noexcept;

    [[nodiscard]] std::shared_ptr<Transform> GetTransform() const noexcept;

    // Events
    virtual void Update();
    virtual void Draw();
    virtual void Draw2D();

private:
    std::weak_ptr<Actor> actor_;
    bool isInitialized_{false};
};
