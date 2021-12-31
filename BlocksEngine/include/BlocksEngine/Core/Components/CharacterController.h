// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: CharacterController.h

#pragma once
#include "Component.h"
#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    class CharacterController;
}

class BlocksEngine::CharacterController final : public Component
{
public:
    CharacterController();

    CharacterController(const CharacterController&) = delete;
    CharacterController& operator=(const CharacterController&) = delete;

    CharacterController(CharacterController&&) = delete;
    CharacterController& operator=(CharacterController&&) = delete;

    ~CharacterController() override;

    void Start() override;
    void PhysicsUpdated() override;

    void Move(const Vector3<float>& motion) const;

private:
    physx::PxController* controller_{nullptr};

    void OnMove(const Vector3<float>& newPos) const;
};
