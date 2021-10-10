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

Window& Component::GetWindow() const noexcept
{
    return GetActor().GetWindow();
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
