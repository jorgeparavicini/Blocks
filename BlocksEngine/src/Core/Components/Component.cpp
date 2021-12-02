#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Component.h"

#include <thread>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Component::Component(std::weak_ptr<Actor> actor, const uint32_t index, const  uint32_t generation)
    : Entity{index, generation},
      actor_{std::move(actor)}
{
    assert("Component must be added on the main thread" && GetGame()->GetMainThreadId() == std::this_thread::get_id());
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
