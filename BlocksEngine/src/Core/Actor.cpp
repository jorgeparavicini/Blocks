#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Actor.h"

#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

constexpr uint32_t Actor::INDEX_BITS = 24;
constexpr uint32_t Actor::INDEX_MASK = (1 << INDEX_BITS) - 1;
constexpr uint32_t Actor::GENERATION_BITS = 8;
constexpr uint32_t Actor::GENERATION_MASK = (1 << GENERATION_BITS) - 1;
constexpr uint32_t Actor::MINIMUM_FREE_INDICES = 1024;

Actor::Actor(std::weak_ptr<Game> game, uint32_t index, uint32_t generation, std::wstring name)
    : id_{index & INDEX_MASK | (generation & GENERATION_MASK) << INDEX_BITS},
      name_{std::move(name)},
      game_{std::move(game)},
      pTransform_{std::make_shared<Transform>()}
{
}


inline bool Actor::operator==(const Actor& actor) const
{
    return id_ == actor.id_;
}

inline bool Actor::operator!=(const Actor& actor) const
{
    return id_ != actor.id_;
}

const std::wstring& Actor::GetName() const noexcept
{
    return name_;
}

std::shared_ptr<Game> Actor::GetGame() const noexcept
{
    return game_.lock();
}

std::shared_ptr<Transform> Actor::GetTransform() const noexcept
{
    return pTransform_;
}


void Actor::Update() const
{
    pTransform_->UpdateMatrix();

    for (auto& pComponent : pComponents_)
    {
        pComponent->Update();
    }
}

void Actor::Render() const
{
    for (auto& pComponent : pComponents_)
    {
        pComponent->Draw();
    }
}

void Actor::Render2D() const
{
    for (auto& pComponent : pComponents_)
    {
        pComponent->Draw2D();
    }
}
