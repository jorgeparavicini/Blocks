#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Actor.h"

#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Actor::Actor(std::weak_ptr<Game> game, std::wstring name)
    : name_{std::move(name)},
      game_{std::move(game)},
      pTransform_{std::make_shared<Transform>()}
{
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
