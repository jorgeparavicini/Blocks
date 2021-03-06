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
#include <queue>

#include "Entity.h"
#include "robin_hood.h"
#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Graphics/Graphics.h"
#include "BlocksEngine/Main/Game.h"
#include "BlocksEngine/Utility/Concepts.h"

namespace BlocksEngine
{
    class Game;
    class Actor;
}

class BlocksEngine::Actor final : public Entity, public std::enable_shared_from_this<Actor>
{
public:
    struct ActorHash
    {
        std::size_t operator()(const Actor& actor) const
        {
            return robin_hood::hash<int>()(actor.id_);
        }
    };

    //------------------------------------------------------------------------------
    // Destructors | Copy | Move
    //------------------------------------------------------------------------------

    ~Actor() = default;

    Actor(const Actor& actor) = delete;
    Actor& operator=(const Actor& actor) = delete;

    Actor(const Actor&& actor) = delete;
    Actor& operator=(const Actor&& actor) = delete;

    //------------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------------

    bool operator==(const Actor& actor) const;

    bool operator!=(const Actor& actor) const;

    //------------------------------------------------------------------------------
    // Member accessors
    //------------------------------------------------------------------------------

    [[nodiscard]] const std::wstring& GetName() const noexcept;
    [[nodiscard]] std::shared_ptr<Game> GetGame() const noexcept;
    [[nodiscard]] std::shared_ptr<Transform> GetTransform() const noexcept;

    //------------------------------------------------------------------------------
    // Events
    //------------------------------------------------------------------------------

    void Update() const;
    void Render() const;
    void Render2D() const;
    void PhysicsUpdated() const;


    //------------------------------------------------------------------------------
    // ECS
    //------------------------------------------------------------------------------

    /**
     * Creates a component and adds it to the component list.
     */
    template <Derived<Component> T, class... Args>
    std::shared_ptr<T> AddComponent(Args&&... args)
    {
        assert(
            "Components must be added on the main thread" && std::this_thread::get_id() == GetGame()->GetMainThreadId(
            ));
        uint32_t index;

        // If there are already enough free indices to start using them
        if (freeIndices_.size() > MINIMUM_FREE_INDICES)
        {
            index = freeIndices_.front();
            freeIndices_.pop();
        }
        else
        {
            generations_.push_back(0);
            index = static_cast<uint32_t>(generations_.size()) - 1;

            assert(index < 1u << Actor::INDEX_BITS);
        }

        std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
        component->Initialize(shared_from_this(), index, static_cast<uint32_t>(generations_[index]));

        if (index == pComponents_.size())
        {
            pComponents_.push_back(component);
            component->Start();
            return component;
        }

        if (index >= pComponents_.size())
        {
            pComponents_.resize(static_cast<unsigned long long>(index) + 1);
        }

        pComponents_[index] = component;
        component->Start();
        return component;
    }


    // No order guaranteed
    template <Derived<Component> T>
    [[nodiscard]] std::shared_ptr<T> GetComponent() const
    {
        // TODO: verify that the reference doesn't fuck up the smart pointer
        for (auto& pComponent : pComponents_)
        {
            if (auto c = std::dynamic_pointer_cast<T>(pComponent)) return c;
        }
        return nullptr;
    }

    // No order guaranteed
    template <Derived<Component> T>
    [[nodiscard]] std::vector<std::shared_ptr<T>> GetComponents() const
    {
        std::vector<std::shared_ptr<T>> result{};

        for (auto& pComponent : pComponents_)
        {
            if (auto c = std::dynamic_pointer_cast<T>(pComponent)) result.push_back(c);
        }

        return result;
    }

    //------------------------------------------------------------------------------
    // Friends
    //------------------------------------------------------------------------------

    friend Game;
    friend Component;

    friend bool operator==(const Actor& actor1, const Actor& actor2)
    {
        return actor1.id_ == actor2.id_;
    }

private:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit Actor(std::weak_ptr<Game> game, uint32_t index,
                   uint32_t generation, std::wstring name);

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::wstring name_;
    std::weak_ptr<Game> game_;
    std::shared_ptr<Transform> transform_{};
    std::vector<std::shared_ptr<Component>> pComponents_{};
    std::queue<std::shared_ptr<Component>> pDestroyQueue_{};

    // Event Sets
    // TODO: Rename as they are not queues per se but we don't want to allow multiple values
    robin_hood::unordered_set<uint32_t> updateQueue_{};
    robin_hood::unordered_set<uint32_t> renderQueue_{};
    robin_hood::unordered_set<uint32_t> render2DQueue_{};
    robin_hood::unordered_set<uint32_t> physicsUpdatedQueue_{};

    // Component states
    // TODO: We basically repeat stuff from the game class. Maybe it can be refactored
    std::vector<uint8_t> generations_{};
    std::queue<uint32_t> freeIndices_{};

    void SetEventTypeForComponent(const Component& component, EventType eventTypes);
    void SetComponentEnabled(const Component& component, bool enabled);
};
