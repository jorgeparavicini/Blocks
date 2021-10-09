// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Transform.h

#pragma once
#include "DirectXMath.h"
#include "Vector3.h"

namespace BlocksEngine
{
    class Transform;
}

class BlocksEngine::Transform
{
public:
    explicit Transform(Vector3 position = Vector3::Zero(), Vector3 rotation = Vector3::Zero(),
                       Vector3 scale = Vector3::One());
    [[nodiscard]] DirectX::XMMATRIX GetMatrix() const;
    void SetPosition(Vector3 position) noexcept;

private:
    Vector3 position_;
    Vector3 rotation_;
    Vector3 scale_;
};
