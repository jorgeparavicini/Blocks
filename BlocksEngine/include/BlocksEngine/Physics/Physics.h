// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Physics.h

#pragma once
#include <PxPhysicsAPI.h>

namespace BlocksEngine
{
    class Physics;
}

#define PX_RELEASE(x) if(x) { (x)->release(); (x) = NULL; }

class BlocksEngine::Physics
{
public:
    explicit Physics(bool recordMemoryAllocations = true, bool connectVisualDebugger = true);

    Physics(const Physics&) = delete;
    Physics& operator=(const Physics&) = delete;

    Physics(Physics&&) = delete;
    Physics& operator=(Physics&&) = delete;

    ~Physics();

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void Update(float stepTime) const;

    physx::PxPhysics& GetPhysics();
    physx::PxScene& GetScene();
    physx::PxCooking& GetCooking();

    [[nodiscard]] physx::PxMaterial& DefaultMaterial();

private:
    physx::PxPhysics* physics_;
    physx::PxFoundation* foundation_;
    physx::PxDefaultCpuDispatcher* cpuDispatcher_;
    physx::PxCooking* cooking_;

    physx::PxScene* scene_;
    physx::PxMaterial* defaultMaterial_;

    physx::PxDefaultAllocator defaultAllocator_;
    physx::PxDefaultErrorCallback defaultErrorCallback_;

    physx::PxPvd* pvd_{nullptr};
};
