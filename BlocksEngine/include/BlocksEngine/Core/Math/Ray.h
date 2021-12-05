// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Ray.h

#pragma once

namespace BlocksEngine
{
    struct Ray;
}

struct BlocksEngine::Ray
{
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    Ray(const Vector3<float>& p1, const Vector3<float>& p2, float maxFraction = 1.0f);

    Ray(const Ray& ray);
    Ray& operator=(const Ray& ray);

    Ray(Ray&& ray);
    Ray& operator=(Ray&& ray);

    ~Ray() = default;


    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    /**
     * \brief The first point of the ray (origin) in world-space
     */
    Vector3<float> point1;

    /**
     * \brief Second point of the ray in (target) world-space
     */
    Vector3<float> point2;

    /**
     * \brief The maximum fraction value
     */
    float maxFraction;
};
