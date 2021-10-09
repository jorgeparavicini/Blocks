﻿#include "BlocksEngine/pch.h"
#include "BlocksEngine/Actor.h"

#include "BlocksEngine/Game.h"

using namespace BlocksEngine;

Actor::Actor(Game& game)
    : game_{game},
      pTransform_{std::make_unique<Transform>()}
{
}

Transform& Actor::GetTransform() const noexcept
{
    return *pTransform_;
}

const Game& Actor::GetGame() const noexcept
{
    return game_;
}

const Graphics& Actor::GetGraphics() const noexcept
{
    return GetGame().GetGraphics();
}

void Actor::Update() const
{
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
