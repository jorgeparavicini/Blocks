#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Transform.h"

using namespace DirectX;
using namespace BlocksEngine;

// TODO: Testing

Transform::Transform(const Vector3<float>& position,
                     const Quaternion& orientation,
                     const Vector3<float>& scale)
    : position_{position},
      orientation_{orientation},
      scale_{scale}
{
}

Matrix Transform::GetMatrix() const noexcept
{
    return Matrix::CreateTranslation(GetPosition())
        * Matrix::CreateFromQuaternion(GetOrientation())
        * Matrix::CreateScale(scale_);
}

const Vector3<float>& Transform::GetPosition() const noexcept
{
    return position_;
}

const Vector3<float>& Transform::GetLocalPosition() const noexcept
{
    const auto p = parent_.lock();
    if (!p)
    {
        return GetPosition();
    }
    return GetPosition() - p->GetLocalPosition();
}

const Quaternion& Transform::GetOrientation() const noexcept
{
    return orientation_;
}

const Quaternion& Transform::GetLocalOrientation() const noexcept
{
    const auto p = parent_.lock();
    if (!p)
    {
        return GetOrientation();
    }
    return GetOrientation() / p->GetLocalOrientation();
}

const Vector3<float>& Transform::GetScale() const noexcept
{
    return scale_;
}

const Vector3<float>& Transform::GetLocalScale() const noexcept
{
    const auto p = parent_.lock();
    if (!p)
    {
        return GetScale();
    }
    // TODO: Verify
    return scale_ / p->GetLocalScale();
}

std::weak_ptr<Transform> Transform::GetChild(const size_t index) const noexcept
{
    return children_[index];
}

void Transform::SetPosition(const Vector3<float>& position) noexcept
{
    const auto diff = position - position_;
    position_ = position;

    for (auto& child : children_)
    {
        const auto c = child.lock();
        c->SetPosition(diff + c->GetPosition());
    }

    moved_(position);
}

void Transform::SetLocalPosition(const Vector3<float>& position) noexcept
{
    auto parentPos = Vector3<float>::Zero;
    if (const auto p = parent_.lock())
    {
        parentPos = p->GetPosition();
    }
    SetPosition(position + parentPos);
}

void Transform::SetOrientation(const Quaternion& orientation) noexcept
{
    Quaternion o;
    orientation.Normalize(o);
    Quaternion inverted;
    o.Inverse(inverted);
    const auto diff = orientation_ * inverted;
    diff.Inverse(inverted);
    orientation_ = o;

    for (auto& child : children_)
    {
        const auto c = child.lock();
        c->SetOrientation(c->GetOrientation() * inverted);
    }

    rotated_(o);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Transform::SetLocalOrientation(const Quaternion& orientation) noexcept
{
    auto parentOrientation = Quaternion::Identity;
    if (const auto p = parent_.lock())
    {
        parentOrientation = p->GetOrientation();
    }
    SetOrientation(orientation * parentOrientation);
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
    // TODO: Not sure if this is enough, we only check if it is the exact same object
    return this == &transform;
}

boost::signals2::connection Transform::AddSignalOnMove(const MoveSignal::slot_type& slot) noexcept
{
    return moved_.connect(slot);
}

boost::signals2::connection Transform::AddSignalOnRotate(const RotateSignal::slot_type& slot) noexcept
{
    return rotated_.connect(slot);
}
