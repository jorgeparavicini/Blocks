// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: RaycastInfo.h

#pragma once

#include "BlocksEngine/Core/Math/Ray.h"
#include "BlocksEngine/Physics/Body/CollisionBody.h"

namespace BlocksEngine
{
    struct RaycastInfo;
    class RaycastCallback;
    struct RaycastTest;
}

struct BlocksEngine::RaycastInfo
{
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    RaycastInfo();

    RaycastInfo(const RaycastInfo&) = delete;
    RaycastInfo& operator=(const RaycastInfo&) = delete;

    RaycastInfo(RaycastInfo&&) = delete;
    RaycastInfo& operator=(RaycastInfo&&) = delete;

    ~RaycastInfo() = default;


    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    Vector3<float> worldPoint;

    Vector3<float> worldNormal;

    float hitFraction;

    int meshSubpart;

    int triangleIndex;

    std::shared_ptr<CollisionBody> body;

    std::shared_ptr<Collider> collider;
};

class BlocksEngine::RaycastCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------


    RaycastCallback(const RaycastCallback&) = delete;
    RaycastCallback& operator=(const RaycastCallback&) = delete;

    RaycastCallback(RaycastCallback&&) = delete;
    RaycastCallback& operator=(RaycastCallback&&) = delete;

    virtual ~RaycastCallback() = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    /**
     * @bried  This method will be called for each collider that is hit by the
               ray. You cannot make any assumptions about the order of the
               calls. You should use the return value to control the continuation
               of the ray. The returned value is the next maxFraction value to use.
               If you return a fraction of 0.0, it means that the raycast should
               terminate. If you return a fraction of 1.0, it indicates that the
               ray is not clipped and the ray cast should continue as if no hit
               occurred. If you return the fraction in the parameter (hitFraction
               value in the RaycastInfo object), the current ray will be clipped
               to this fraction in the next queries. If you return -1.0, it will
               ignore this collider and continue the ray cast.
     * @param raycastInfo Information about the raycast hit
     * @return Value that controls the continuation of the ray after a hit
     */
    virtual float NotifyRaycastHit(const RaycastInfo& raycastInfo) = 0;
};

struct BlocksEngine::RaycastTest
{
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    RaycastTest(std::unique_ptr<RaycastCallback> callback);

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    float RaycastAgainstShape(const Collider& shape, const Ray& ray);

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::unique_ptr<RaycastCallback> userCallback;
};
