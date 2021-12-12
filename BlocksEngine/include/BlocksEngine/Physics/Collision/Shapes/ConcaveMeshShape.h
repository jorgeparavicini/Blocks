// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConcaveMeshShape.h

#pragma once
#include "ConcaveShape.h"
#include "BlocksEngine/Physics/Collision/Broadphase/DynamicAABBTree.h"

namespace BlocksEngine
{
    class ConvexTriangleAABBOverlapCallback;
    class ConcaveMeshRaycastCallback;
    class ConcaveMeshShape;
}

class BlocksEngine::ConvexTriangleAABBOverlapCallback final : public DynamicAABBTreeOverlapCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConvexTriangleAABBOverlapCallback(TriangleCallback& triangleCallback, const ConcaveMeshShape& concaveMeshShape,
                                      const DynamicAABBTree& dynamicAABBTree);


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void NotifyOverlappingNode(int nodeId) override;

private:
    TriangleCallback& triangleTestCallback_;

    const ConcaveMeshShape& concaveMeshShape_;

    const DynamicAABBTree& dynamicAABBTree_;
};


class BlocksEngine::ConcaveMeshRaycastCallback final : public DynamicAABBTreeRaycastCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConcaveMeshRaycastCallback(const DynamicAABBTree& dynamicAABBTree,
                               const ConcaveMeshShape& concaveMeshShape,
                               Collider* collider, RaycastInfo& raycastInfo,
                               const Ray& ray, const Vector3<float>& meshScale);


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    float RaycastBroadPhaseShape(int32_t nodeId, const Ray& ray) override;


    void RaycastTriangles();

    [[nodiscard]] bool IsHit() const;

private:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::vector<int> hitAABBNodes_;
    const DynamicAABBTree& dynamicAABBTree_;
    const ConcaveMeshShape& concaveMeshShape_;
    Collider* collider_;
    RaycastInfo& raycastInfo_;
    const Ray& ray_;
    bool isHit_;
    const Vector3<float>& meshScale_;
};


class BlocksEngine::ConcaveMeshShape
{
public:

protected:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConcaveMeshShape(TriangleMesh* triangleMesh, const Vector3<float>& scaling = Vector3<float>::One);
};


inline void BlocksEngine::ConvexTriangleAABBOverlapCallback::NotifyOverlappingNode(const int nodeId)
{
    const int32_t* data = dynamicAABBTree_.GetNodeDataInt(nodeId);

    Vector3<float> trianglePoints[3];
    concaveMeshShape_.GetTriangleVertices(data[0], data[1], trianglePoints);

    Vector3 verticesNormals[3];
    concaveMeshShape_.GetTriangleVerticesNormals(data[0], data[1], verticesNormals);

    triangleTestCallback_.TestTriangle(trianglePoints, verticesNormals,
                                       concaveMeshShape_.ComputeTriangleShapeId(data[0], data[1]))
}

inline bool BlocksEngine::ConcaveMeshRaycastCallback::IsHit() const
{
    return isHit_;
}
