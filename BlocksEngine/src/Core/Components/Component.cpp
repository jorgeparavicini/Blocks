#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Component.h"

#include "BlocksEngine/Core/Actor.h"

using namespace BlocksEngine;

Component::Component(std::weak_ptr<Actor> actor)
    : actor_{std::move(actor)}
{
}

std::shared_ptr<Game> Component::GetGame() const noexcept
{
    return GetActor()->GetGame();
}

std::shared_ptr<Actor> Component::GetActor() const noexcept
{
    return actor_.lock();
}

std::shared_ptr<Transform> Component::GetTransform() const noexcept
{
    return GetActor()->GetTransform();
}

void Component::Update()
{
}

void Component::Draw()
{
}

void Component::Draw2D()
{
}
