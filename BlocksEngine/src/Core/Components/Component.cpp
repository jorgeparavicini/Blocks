#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Component.h"

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Exceptions/EngineException.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

std::shared_ptr<Game> Component::GetGame() const
{
    return GetActor()->GetGame();
}

std::shared_ptr<Actor> Component::GetActor() const
{
    // TODO: Test
    if (actor_.expired())
    {
        throw ENGINE_EXCEPTION("Called Get Actor before Component was initialized");
    }
    return actor_.lock();
}

std::shared_ptr<Transform> Component::GetTransform() const noexcept
{
    return GetActor()->GetTransform();
}

bool Component::IsEnabled() const noexcept
{
    return enabled_;
}

void Component::SetEnabled(const bool enabled)
{
    enabled_ = enabled;
    GetActor()->SetComponentEnabled(*this, enabled);
}

void Component::Start()
{
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

EventType Component::GetEventTypes() const noexcept
{
    return eventTypes_;
}

void Component::SetEventTypes(const EventType eventTypes)
{
    eventTypes_ = eventTypes;
    GetActor()->SetEventTypeForComponent(*this, eventTypes);
}

void Component::Initialize(std::weak_ptr<Actor> actor, const uint32_t index, const uint32_t generation)
{
    actor_ = std::move(actor);
    SetId(index, generation);
}
