// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetTransform.h

#pragma once

#include "BlocksEngine/Core/Math/Matrix.h"
#include "BlocksEngine/Core/Math/Quaternion.h"
#include "BlocksEngine/Core/Math/Vector3.h"

// TODO: Move to math
namespace BlocksEngine
{
    class Transform;
}

class BlocksEngine::Transform
{
public:
    Transform();

    explicit Transform(const Vector3<float>& position,
                       const Quaternion& orientation,
                       const Vector3<float>& scale);

    Transform(const Vector3<float>& position, const Quaternion& orientation);

    [[nodiscard]] const Matrix& GetMatrix() const noexcept;
    [[nodiscard]] const Vector3<float>& GetPosition() const noexcept;
    [[nodiscard]] const Quaternion& GetRotation() const noexcept;
    [[nodiscard]] const Vector3<float>& GetScale() const noexcept;
    [[nodiscard]] Transform GetInverse() const noexcept;

    void SetPosition(Vector3<float> position) noexcept;
    void SetRotation(Quaternion rotation) noexcept;
    void SetScale(Vector3<float> scale) noexcept;

    void UpdateMatrix() noexcept;

    [[nodiscard]] Transform operator*(const Transform& transform) const;
    [[nodiscard]] Vector3<float> operator*(const Vector3<float>& vector) const;

private:
    Vector3<float> position_;
    Quaternion rotation_;
    Vector3<float> scale_;

    Matrix matrix_;
};

inline BlocksEngine::Transform BlocksEngine::Transform::GetInverse() const noexcept
{
    Quaternion invQuaternion;
    rotation_.Inverse(invQuaternion);
    return {invQuaternion * -position_, invQuaternion};
}

inline BlocksEngine::Transform BlocksEngine::Transform::operator*(const Transform& transform) const
{
    return {position_ + rotation_ * transform.position_, rotation_ * transform.rotation_};
}

inline BlocksEngine::Vector3<float> BlocksEngine::Transform::operator*(const Vector3<float>& vector) const
{
    return rotation_ * vector + position_;
}
