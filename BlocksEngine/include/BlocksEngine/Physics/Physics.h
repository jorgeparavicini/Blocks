// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetPhysics.h

#pragma once
#include "Body/CollisionBody.h"

namespace BlocksEngine
{
    class Physics;
}

class BlocksEngine::Physics
{
public:
    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    struct PhysicsSettings
    {
        //------------------------------------------------------------------------------
        // Constructors, Destructors, Assignment & Move
        //------------------------------------------------------------------------------

        PhysicsSettings();


        //------------------------------------------------------------------------------
        // Fields
        //------------------------------------------------------------------------------

        Vector3<float> gravity;

        float persistentContactDistanceThreshold;

        float defaultFrictionCoefficient;

        float defaultBounciness;

        float restitutionVelocityThreshold;

        float defaultRollingResistance;

        bool isSleepingEnabled;

        unsigned int defaultVelocitySolverIterationsCount;

        unsigned int defaultPositionSolverIterationsCount;

        float defaultTimeBeforeSleep;

        float defaultSleepLinearVelocity;

        float defaultSleepAngularVelocity;

        unsigned int maxContactManifoldsCount;

        float cosAngleSimilarContactManifold;


        //------------------------------------------------------------------------------
        // Methods
        //------------------------------------------------------------------------------

        [[nodiscard]] std::string ToString() const;
    };

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void Update(float deltaTime);

protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    PhysicsSettings settings_;

    std::vector<CollisionBody> collisionBodies_;

    std::vector<RigidBody> rigidBodies_;
};
