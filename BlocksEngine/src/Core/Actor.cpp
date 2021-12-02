#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Actor.h"

#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;


Actor::Actor(std::weak_ptr<Game> game, const uint32_t index, uint32_t generation, std::wstring name)
    : Entity{index, generation},
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

void Actor::CreateComponents()
{
    while(!pNewComponentQueue_.empty())
    {
        std::shared_ptr<Component> component = pNewComponentQueue_.front();
        pNewComponentQueue_.pop();

        const uint32_t index = component->GetIndex();

        if (index == pComponents_.size())
        {
            pComponents_.push_back(std::move(component));
            continue;
        }

        if (index >= pComponents_.size())
        {
            pComponents_.reserve(static_cast<unsigned long long>(index) + 1);
        }

        component->Start();
        pComponents_[index] = std::move(component);
    }
}

void Actor::SetEventTypeForComponent(const Component& component, EventType eventTypes)
{
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

    GetGame()->UpdateEventTypeForActor(*this, componentEvent);
}

// TODO: Destroy Actor request

void Actor::Update() const
{
    // TODO: Maybe move to internal update? Not sure if this must be updated, if other actors modify this transform it could be a use case
    pTransform_->UpdateMatrix();

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
