﻿#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Transform.h"

using namespace DirectX;
using namespace BlocksEngine;


Transform::Transform(Vector3<float> position, Quaternion rotation, Vector3<float> scale)
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

Vector3<float>& Transform::GetPosition() noexcept
{
    return position_;
}

Quaternion& Transform::GetRotation() noexcept
{
    return rotation_;
}

Vector3<float>& Transform::GetScale() noexcept
{
    return scale_;
}

void Transform::SetPosition(Vector3<float> position) noexcept
{
    position_ = std::move(position);
}

void Transform::SetRotation(Quaternion rotation) noexcept
{
    rotation_ = std::move(rotation);
}

void Transform::SetScale(const Vector3<float> scale) noexcept
{
    scale_ = scale;
}

void Transform::UpdateMatrix() noexcept
{
    matrix_ = Matrix::CreateTranslation(position_)
        * Matrix::CreateFromQuaternion(rotation_)
        * Matrix::CreateScale(scale_);
}
