// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Component.h

#pragma once
#include "BlocksEngine/Entity.h"

namespace BlocksEngine
{
    class Transform;
    class Actor;
    class Component;
}

class BlocksEngine::Component : public Entity
{
public:
    /**
     * Subclasses need to put Actor& as first parameter.
     */
    explicit Component(Actor& actor);
    virtual ~Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(const Component&&) = delete;
    Component& operator=(const Component&&) = delete;

    /**
     * Gets the actor this component is attached to.
     */
    [[nodiscard]] const Actor& GetActor() const noexcept;

    [[nodiscard]] Transform& GetTransform() const noexcept;

    // Events
    virtual void Update();
    virtual void Draw();

private:
    Actor& actor_;
    bool isInitialized_{false};
};
