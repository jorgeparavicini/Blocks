#include "BlocksEngine/pch.h"
#include "BlocksEngine/Transform.h"

using namespace DirectX;
using namespace BlocksEngine;


Transform::Transform(Vector3 position, Quaternion rotation, Vector3 scale)
    : position_{std::move(position)},
      rotation_{std::move(rotation)},
      scale_{std::move(scale)}
{
    UpdateMatrix();
}

const Matrix& Transform::GetMatrix() const noexcept
{
    return matrix_;
}

Vector3& Transform::GetPosition() noexcept
{
    return position_;
}

Quaternion& Transform::GetRotation() noexcept
{
    return rotation_;
}

Vector3& Transform::GetScale() noexcept
{
    return scale_;
}

void Transform::SetPosition(Vector3 position) noexcept
{
    position_ = std::move(position);
}

void Transform::SetRotation(Quaternion rotation) noexcept
{
    rotation_ = std::move(rotation);
}

void Transform::SetScale(const Vector3 scale) noexcept
{
    scale_ = scale;
}

void Transform::UpdateMatrix() noexcept
{
    matrix_ = Matrix::CreateTranslation(position_)
        * Matrix::CreateFromQuaternion(rotation_)
        * Matrix::CreateScale(scale_);
}
