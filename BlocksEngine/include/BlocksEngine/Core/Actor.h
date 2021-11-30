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
#include "BlocksEngine/Utility/Concepts.h"

namespace BlocksEngine
{
    class Game;
    class Actor;
}

class BlocksEngine::Actor final : public std::enable_shared_from_this<Actor>
{
public:
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

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] uint32_t GetIndex() const;
    [[nodiscard]] uint32_t GetGeneration() const;

    //------------------------------------------------------------------------------
    // ECS
    //------------------------------------------------------------------------------

    /**
     * Creates a component and adds it to the component list.
     */
    template <Derived<Component> T, class... Args>
    std::shared_ptr<T> AddComponent(Args&&... args)
    {
        std::shared_ptr<T> component = std::make_shared<T>(shared_from_this(), std::forward<Args>(args)...);
        pComponents_.insert(std::dynamic_pointer_cast<Component>(component));
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

private:
    /**
     * \brief Number of bits reserved for the index
     */
    static const uint32_t INDEX_BITS;

    /**
     * \brief Mask for the index part of the id
     */
    static const uint32_t INDEX_MASK;

    /**
     * \brief Number of bits reserved for teh generation number
     */
    static const uint32_t GENERATION_BITS;

    /**
     * \brief Mask for the generation part of the id
     */
    static const uint32_t GENERATION_MASK;

    /**
     * \brief Minimum of free indices in the queue before we reuse one from the queue
     */
    static const uint32_t MINIMUM_FREE_INDICES;

    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit Actor(std::weak_ptr<Game> game, uint32_t index, uint32_t generation, std::wstring name);

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    uint32_t id_;
    std::wstring name_;
    std::weak_ptr<Game> game_;
    std::shared_ptr<Transform> pTransform_{};
    std::unordered_set<std::shared_ptr<Component>> pComponents_{};
};

inline uint32_t BlocksEngine::Actor::GetIndex() const
{
    return id_ & INDEX_MASK;
}

inline uint32_t BlocksEngine::Actor::GetGeneration() const
{
    return id_ >> INDEX_BITS & GENERATION_MASK;
}
