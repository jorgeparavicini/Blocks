// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: PlayerMovement.h

#pragma once
#include <memory>

#include "BlocksEngine/Core/Components/CharacterController.h"
#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Core/Math/Math.h"

namespace Blocks
{
    class PlayerMovement;
}

class Blocks::PlayerMovement final : public BlocksEngine::Component
{
public:
    explicit PlayerMovement(std::weak_ptr<BlocksEngine::CharacterController> controller);

    void Start() override;
    void Update() override;
private:
    std::weak_ptr<BlocksEngine::CharacterController> controller_;
    BlocksEngine::Vector3<float> velocity_{BlocksEngine::Vector3<float>::Zero};

    float keyboardRotationSpeed_{1.0f};
    float rotationSpeed_{2.0f};
    float moveSpeed_{4.0f};
    float jumpHeight_{1.1f};
};
