// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConcaveMeshShape.h

#pragma once
#include "BlocksEngine/Physics/Collision/Broadphase/DynamicAABBTree.h"

namespace BlocksEngine
{
    class ConvexTriangleAABBOverlapCallback;
    class ConcaveMeshShape;
}

class BlocksEngine::ConvexTriangleAABBOverlapCallback : public DynamicAABBTreeOverlapCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConvexTriangleAABBOverlapCallback(TriangleCallb)
};

class BlocksEngine::ConcaveMeshShape
{
public:
};
