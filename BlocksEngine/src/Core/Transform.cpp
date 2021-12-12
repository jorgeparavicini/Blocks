#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Transform.h"

using namespace DirectX;
using namespace BlocksEngine;

Transform::Transform()
    : Transform{Vector3<float>::Zero, Quaternion::Identity, Vector3<float>::One}
{
}


Transform::Transform(const Vector3<float>& position, const Quaternion& orientation, const Vector3<float>& scale)
    : position_{position},
      rotation_{orientation},
      scale_{scale}
{
    UpdateMatrix();
}

Transform::Transform(const Vector3<>& position, const Quaternion& orientation)
    : position_{position},
      rotation_{orientation},
      scale_{Vector3<float>::One}
{
}

const Matrix& Transform::GetMatrix() const noexcept
{
    return matrix_;
}

const Vector3<float>& Transform::GetPosition() const noexcept
{
    return position_;
}

// TODO: Quaternion should be renamed orientation. Euler angles are rotation
const Quaternion& Transform::GetRotation() const noexcept
{
    return rotation_;
}

const Vector3<float>& Transform::GetScale() const noexcept
{
    return scale_;
}

void Transform::SetPosition(Vector3<float> position) noexcept
{
    position_ = std::move(position);
    UpdateMatrix();
}

void Transform::SetRotation(Quaternion rotation) noexcept
{
    rotation_ = std::move(rotation);
    UpdateMatrix();
}

void Transform::SetScale(const Vector3<float> scale) noexcept
{
    scale_ = scale;
    UpdateMatrix();
}

void Transform::UpdateMatrix() noexcept
{
    matrix_ = Matrix::CreateTranslation(position_)
        * Matrix::CreateFromQuaternion(rotation_)
        * Matrix::CreateScale(scale_);
}
