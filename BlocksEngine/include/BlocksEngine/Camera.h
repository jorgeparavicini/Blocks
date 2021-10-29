// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Camera.h

#pragma once

#include <DirectXMath.h>

#include "Actor.h"
#include "Matrix.h"

namespace BlocksEngine
{
    class Camera;
}

class BlocksEngine::Camera final : public Component
{
public:
    explicit Camera(Actor& actor);

    [[nodiscard]] Matrix ViewProjection() const noexcept;
    [[nodiscard]] Matrix View() const noexcept;
    [[nodiscard]] Matrix Projection() const noexcept;

    void OnWindowResized(int width, int height) noexcept;

    void Update() override;


private:
    float lastX_{0};
    float lastY_{0};
    Vector3 rotation_{0, 0, 0};

    float moveSpeed_{10.0f};
    float rotationSpeed_{10.0f};
    float keyboardRotationSpeed_{10.0f};
    float x_{0.1f};
    Matrix projection_;
    boost::signals2::connection windowResizedConnection_;

    [[nodiscard]] float ClampAngle(float angle, float min, float max) const;

    [[nodiscard]] DirectX::XMMATRIX CalculateProjection() const noexcept;
};
