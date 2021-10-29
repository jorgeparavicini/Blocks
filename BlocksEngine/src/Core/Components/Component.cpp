#include "BlocksEngine/pch.h"
#include "BlocksEngine/Component.h"

#include "BlocksEngine/Actor.h"

using namespace BlocksEngine;

Component::Component(Actor& actor)
    : actor_{actor}
{
}

Actor& Component::GetActor() const noexcept
{
    return actor_;
}

Game& Component::GetGame() const noexcept
{
    return actor_.GetGame();
}

Transform& Component::GetTransform() const noexcept
{
    return actor_.GetTransform();
}

void Component::Update()
{
}

void Component::Draw()
{
}
