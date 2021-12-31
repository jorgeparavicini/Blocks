#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Transform.h"

using namespace DirectX;
using namespace BlocksEngine;


Transform::Transform(physx::PxRigidActor* actor)
    : actor_{actor},
      scale_{Vector3<float>::One}
{
}

Matrix Transform::GetMatrix() const noexcept
{
    return Matrix::CreateTranslation(GetPosition())
        * Matrix::CreateFromQuaternion(GetOrientation())
        * Matrix::CreateScale(scale_);
}

Vector3<float> Transform::GetPosition() const noexcept
{
    return static_cast<Vector3<float>>(actor_->getGlobalPose().p);
}

Vector3<> Transform::GetLocalPosition() const noexcept
{
    const auto p = parent_.lock();
    if (!p)
    {
        return GetPosition();
    }
    return GetPosition() - p->GetLocalPosition();
}

Quaternion Transform::GetOrientation() const noexcept
{
    return static_cast<Quaternion>(actor_->getGlobalPose().q);
}

Quaternion Transform::GetLocalOrientation() const noexcept
{
    // TODO: This is probably wrong
    const auto p = parent_.lock();
    if (!p)
    {
        return GetOrientation();
    }
    return GetOrientation() - p->GetLocalOrientation();
}

Vector3<float> Transform::GetScale() const noexcept
{
    return scale_;
}

Vector3<> Transform::GetLocalScale() const noexcept
{
    const auto p = parent_.lock();
    if (!p)
    {
        return GetScale();
    }
    // TODO: Verify
    return scale_ / p->GetLocalScale();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::SetPosition(const Vector3<float>& position) noexcept
{
    auto t = actor_->getGlobalPose();
    t.p = position;
    actor_->setGlobalPose(t);

    moved_(position);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::SetLocalPosition(const Vector3<float>& position) noexcept
{
    auto parentPos = Vector3<float>::Zero;
    if (const auto p = parent_.lock())
    {
        parentPos = p->GetPosition();
    }
    SetPosition(position + parentPos);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::SetOrientation(const Quaternion& rotation) noexcept
{
    auto t = actor_->getGlobalPose();
    t.q = rotation;
    actor_->setGlobalPose(t);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::SetLocalOrientation(const Quaternion& rotation) noexcept
{
    auto parentOrientation = Quaternion::Identity;
    if (const auto p = parent_.lock())
    {
        parentOrientation = p->GetOrientation();
    }
    // TODO: This is also probably wrong
    SetOrientation(rotation * parentOrientation);
}

void Transform::SetScale(const Vector3<float>& scale) noexcept
{
    scale_ = scale;
}

void Transform::SetLocalScale(const Vector3<float>& scale) noexcept
{
    const auto p = parent_.lock();
    SetScale(p ? scale * p->GetLocalScale() : scale);
}


void Transform::SetParent(std::weak_ptr<Transform> parent)
{
    parent_ = std::move(parent);
}

void Transform::AddChild(std::weak_ptr<Transform> child)
{
    children_.push_back(std::move(child));
}

void Transform::RemoveChild(Transform& transform)
{
    children_.erase(std::ranges::remove_if(
        children_,
        [&transform](const std::weak_ptr<Transform>& ptr)
        {
            const auto t = ptr.lock();
            assert("Child was null" && t);
            return *t.get() == transform;
        }).begin());
}

bool Transform::operator==(const Transform& transform) const noexcept
{
    return actor_ == transform.actor_;
}

boost::signals2::connection Transform::AddSignalOnMove(const MoveSignal::slot_type& slot) noexcept
{
    return moved_.connect(slot);
}

void Transform::SetActor(physx::PxRigidActor* actor)
{
    actor_->release();
    actor_ = actor;
}
