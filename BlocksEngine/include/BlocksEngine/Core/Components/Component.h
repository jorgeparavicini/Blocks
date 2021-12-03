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

// TODO: We have 2 solutions to create components.
// The first being the one where we pass a ComponentArgs struct to each constructor
// this allows us to use constructors to initialize constants or other constructor only initializable variables
// but we have to create a custom constructor for every component subclass which can be a pain in the ass.
// Also we expose engine internals which is a big no no in my opinion.
// The other approach is where the component constructor is empty and base classes can define their constructors as they want.
// The upside is, we don't have to add ComponentArgs to every constructor but the downside is that some
// variables can not be initialized at construction time, as the engine hasn't set up their internals (actor for example or game) yet
// So we would have to be extra careful to not call engine dependent code inside the component constructors
// For these use cases one could use the Start method

class BlocksEngine::Component : public Entity
{
public:
    // TODO: This approach is somewhat cleaner but also allows the end user to change fundamentals in the engine.
    struct ComponentArgs
    {
        std::weak_ptr<Actor> actor;
        uint32_t index;
        uint32_t generation;
    };

    /**
     * Subclasses need to put GetActor& as first parameter.
     */
    explicit Component(ComponentArgs args);

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
