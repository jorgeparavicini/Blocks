// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetTransform.h

#pragma once

#include "Matrix.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace BlocksEngine
{
    class Transform;
}

class BlocksEngine::Transform
{
public:
    explicit Transform(Vector3<float> position = Vector3<float>::Zero,
                       Quaternion rotation = Quaternion::Identity,
                       Vector3<float> scale = Vector3<float>::One);

    [[nodiscard]] const Matrix& GetMatrix() const noexcept;
    [[nodiscard]] Vector3<float>& GetPosition() noexcept;
    [[nodiscard]] Quaternion& GetRotation() noexcept;
    [[nodiscard]] Vector3<float>& GetScale() noexcept;

    void SetPosition(Vector3<float> position) noexcept;
    void SetRotation(Quaternion rotation) noexcept;
    void SetScale(Vector3<float> scale) noexcept;

    void UpdateMatrix() noexcept;

private:
    Vector3<float> position_;
    Quaternion rotation_;
    Vector3<float> scale_;

    Matrix matrix_;
};
