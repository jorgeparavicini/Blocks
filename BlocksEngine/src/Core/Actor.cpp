#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Actor.h"

#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;


Actor::Actor(std::weak_ptr<Game> game, const uint32_t index,
             const uint32_t generation, std::wstring name)
    : Entity{index, generation},
      name_{std::move(name)},
      game_{std::move(game)},
      transform_{std::make_shared<Transform>()}
{
    const physx::PxTransform t{Vector3<float>::Zero, Quaternion::Identity};
    auto& physics = GetGame()->physics_->GetPhysics();
    physx::PxRigidActor* const actor = physics.createRigidStatic(t);
    GetGame()->GetPhysics().GetScene().addActor(*actor);
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
    return transform_;
}

void Actor::SetEventTypeForComponent(const Component& component, EventType eventTypes)
{
    assert(
        "Main loop dependent methods must be called from the main thread" && std::this_thread::get_id() == GetGame()->
        GetMainThreadId());
    // TODO: This is ugly af
    if ((eventTypes & EventType::Update) == EventType::None)
    {
        updateQueue_.erase(component.GetIndex());
    }
    if ((eventTypes & EventType::Render) == EventType::None)
    {
        renderQueue_.erase(component.GetIndex());
    }

    if ((eventTypes & EventType::Render2D) == EventType::None)
    {
        render2DQueue_.erase(component.GetIndex());
    }

    if ((eventTypes & EventType::PhysicsUpdated) == EventType::None)
    {
        physicsUpdatedQueue_.erase(component.GetIndex());
    }

    if ((eventTypes & EventType::Update) == EventType::Update)
    {
        updateQueue_.insert(component.GetIndex());
    }

    if ((eventTypes & EventType::Render) == EventType::Render)
    {
        renderQueue_.insert(component.GetIndex());
    }

    if ((eventTypes & EventType::Render2D) == EventType::Render2D)
    {
        render2DQueue_.insert(component.GetIndex());
    }

    if ((eventTypes & EventType::PhysicsUpdated) == EventType::PhysicsUpdated)
    {
        physicsUpdatedQueue_.insert(component.GetIndex());
    }

    auto componentEvent = EventType::None;
    if (!updateQueue_.empty())
    {
        componentEvent |= EventType::Update;
    }
    if (!renderQueue_.empty())
    {
        componentEvent |= EventType::Render;
    }
    if (!render2DQueue_.empty())
    {
        componentEvent |= EventType::Render2D;
    }
    if (!physicsUpdatedQueue_.empty())
    {
        componentEvent |= EventType::PhysicsUpdated;
    }

    GetGame()->UpdateEventTypeForActor(*this, componentEvent);
}

void Actor::SetComponentEnabled(const Component& component, const bool enabled)
{
    // TODO: Needs to be tested
    if (!enabled)
    {
        updateQueue_.erase(component.GetIndex());
        renderQueue_.erase(component.GetIndex());
        render2DQueue_.erase(component.GetIndex());
        physicsUpdatedQueue_.erase(component.GetIndex());
    }
    else
    {
        SetEventTypeForComponent(component, component.GetEventTypes());
    }
}

// TODO: Destroy Actor request

void Actor::Update() const
{
    for (const int componentId : updateQueue_)
    {
        if (const auto& component = pComponents_[componentId])
        {
            component->Update();
        }
        else
        {
            // TODO: Same as in game don't abort...
            abort();
        }
    }
}

void Actor::Render() const
{
    for (const int componentId : renderQueue_)
    {
        if (const auto& component = pComponents_[componentId])
        {
            component->Draw();
        }
        else
        {
            // TODO: Same as in game don't abort...
            abort();
        }
    }
}

void Actor::Render2D() const
{
    for (const int componentId : render2DQueue_)
    {
        if (const auto& component = pComponents_[componentId])
        {
            component->Draw2D();
        }
        else
        {
            // TODO: Same as in game don't abort...
            abort();
        }
    }
}

void Actor::PhysicsUpdated() const
{
    for (const int componentId : physicsUpdatedQueue_)
    {
        if (const auto& component = pComponents_[componentId])
        {
            component->PhysicsUpdated();
        }
        else
        {
            abort();
        }
    }
}
