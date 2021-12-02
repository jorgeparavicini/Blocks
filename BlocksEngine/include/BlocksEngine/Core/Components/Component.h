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

#include "BlocksEngine/Core/Entity.h"

namespace BlocksEngine
{
    class Transform;
    class Actor;
    class Game;

    class Component;
}

class BlocksEngine::Component : public Entity
{
public:
    /**
     * Subclasses need to put GetActor& as first parameter.
     */
    explicit Component();

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

    // TODO: This method is private to the engine
    // Do not call this method manually
    void Initialize(std::weak_ptr<Actor> actor, uint32_t index, uint32_t generation);

    // Events
    virtual void Start();
    virtual void Update();
    virtual void Draw();
    virtual void Draw2D();

    //------------------------------------------------------------------------------
    // Friends
    //------------------------------------------------------------------------------

    friend bool operator==(const Component& component1, const Component& component2)
    {
        return component1.id_ == component2.id_ && component1.GetActor() == component2.GetActor();
    }

private:
    std::weak_ptr<Actor> actor_;
};
