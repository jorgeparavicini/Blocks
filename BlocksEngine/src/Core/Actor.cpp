#include "BlocksEngine/pch.h"
#include "BlocksEngine/Actor.h"

#include "BlocksEngine/Game.h"

using namespace BlocksEngine;

Actor::Actor(Game& game, std::wstring name)
    : game_{game},
      name_{std::move(name)},
      pTransform_{std::make_unique<Transform>()}
{
}

Transform& Actor::GetTransform() const noexcept
{
    return *pTransform_;
}

Game& Actor::GetGame() const noexcept
{
    return game_;
}

const std::wstring& Actor::GetName() const noexcept
{
    return name_;
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
