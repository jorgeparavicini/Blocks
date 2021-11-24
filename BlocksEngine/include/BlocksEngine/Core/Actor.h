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

#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Graphics/Graphics.h"

namespace BlocksEngine
{
    class Game;
    class Actor;
}

class BlocksEngine::Actor final : public std::enable_shared_from_this<Actor>
{
public:
    explicit Actor(std::weak_ptr<Game> game, std::wstring name);

    [[nodiscard]] const std::wstring& GetName() const noexcept;
    [[nodiscard]] std::shared_ptr<Game> GetGame() const noexcept;
    [[nodiscard]] std::shared_ptr<Transform> GetTransform() const noexcept;

    void Update() const;
    void Render() const;
    void Render2D() const;

    /**
     * Creates a component and adds it to the component list.
     */
    template <class T, class... Args, typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
    decltype(auto) AddComponent(Args&&... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
        pComponents_.insert(std::dynamic_pointer_cast<Component>(component));
        return component;
    }

private:
    std::wstring name_;
    std::weak_ptr<Game> game_;
    std::shared_ptr<Transform> pTransform_{};
    std::unordered_set<std::shared_ptr<Component>> pComponents_{};
};
