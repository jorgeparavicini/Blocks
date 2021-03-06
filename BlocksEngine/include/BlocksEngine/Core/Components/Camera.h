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
#include <boost/signals2/connection.hpp>

#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Core/Math/Matrix.h"
#include "BlocksEngine/Core/Math/Vector3.h"

namespace BlocksEngine
{
    class Camera;
}

class BlocksEngine::Camera final : public Component
{
public:
    [[nodiscard]] Matrix WorldViewProjection() const noexcept;
    [[nodiscard]] Matrix WorldView() const noexcept;
    [[nodiscard]] Matrix Projection() const noexcept;

    void OnWindowResized(int width, int height) noexcept;

    void Start() override;
    void Update() override;


private:
    float lastX_{0};
    float lastY_{0};

    float moveSpeed_{5.0f};
    float rotationSpeed_{1.0f};
    float keyboardRotationSpeed_{1.0f};
    float x_{0.1f};
    Matrix projection_;
    Matrix worldView_;
    Matrix wvp_;
    boost::signals2::connection windowResizedConnection_;

    [[nodiscard]] float ClampAngle(float angle, float min, float max) const;

    [[nodiscard]] DirectX::XMMATRIX CalculateProjection() const noexcept;

    physx::PxController* controller_{nullptr};

    physx::PxVec3 velocity_{};
};
