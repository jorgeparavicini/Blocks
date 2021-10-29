// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Actor.h

#pragma once
#include <memory>
#include <unordered_set>

#include "BlocksEngine/Component.h"
#include "BlocksEngine/Entity.h"
#include "BlocksEngine/Graphics.h"
#include "BlocksEngine/Transform.h"

namespace BlocksEngine
{
    class Game;
    class Actor;
}

class BlocksEngine::Actor final : public Entity
{
public:
    explicit Actor(Game& game, std::string name);

    [[nodiscard]] Transform& GetTransform() const noexcept;
    [[nodiscard]] Game& GetGame() const noexcept;

    void Update() const;
    void Render() const;

    /**
     * Creates a component and adds it to the component list.
     */
    template <class T, class... Args, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    decltype(auto) AddComponent(Args&&... args)
    {
        std::unique_ptr<T> component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& result = *component;
        pComponents_.insert(std::move(component));
        return result;
    }

private:
    Game& game_;
    std::string name_;
    std::unique_ptr<Transform> pTransform_{};
    std::unordered_set<std::unique_ptr<Component>> pComponents_{};
};
