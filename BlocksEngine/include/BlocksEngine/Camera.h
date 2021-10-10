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

namespace BlocksEngine
{
    class Camera;
}

class BlocksEngine::Camera final : public Component
{
public:
    explicit Camera(Actor& actor);

    [[nodiscard]] DirectX::XMMATRIX Projection() const noexcept;

    void OnWindowResized(int width, int height) noexcept;

    void Update() override;

private:
    DirectX::XMMATRIX projection_;

    boost::signals2::connection windowResizedConnection_;
};
