// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Collider.h

#pragma once
#include "Component.h"
#include "BlocksEngine/Graphics/Mesh/Mesh.h"

namespace BlocksEngine
{
    class Collider;
}

// TODO: Split up and don't make all of them take a mesh. Aka we need primitives
class BlocksEngine::Collider final : public Component
{
public:
    Collider(std::vector<physx::PxVec3> vertices, std::vector<int32_t> indices);

    void Start() override;

private:
    std::vector<physx::PxVec3> vertices_;
    std::vector<int32_t> indices_;

    physx::PxRigidActor* actor_{nullptr};
};
