// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConcaveShape.h

#pragma once

#include "CollisionShape.h"
#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    class TriangleCallback;
    class ConcaveShape;
}


class BlocksEngine::TriangleCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    virtual ~TriangleCallback() = default;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    virtual void TestTriangle(const Vector3<float>* trianglePoints, const Vector3<float>* verticesNormals,
                              int shapeId) = 0;
};

class BlocksEngine::ConcaveShape : public CollisionShape
{
public:
protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    TriangleRaycastSide raycastTestType_;
};
