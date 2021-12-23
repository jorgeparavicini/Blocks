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
#include "BlocksEngine/Graphics/Mesh/Mesh.h"
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
                               std::shared_ptr<Collider> collider, RaycastInfo& raycastInfo,
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
    std::shared_ptr<Collider> collider_;
    RaycastInfo& raycastInfo_;
    const Ray& ray_;
    bool isHit_;
    const Vector3<float>& meshScale_;
};


class BlocksEngine::ConcaveMeshShape final : public ConcaveShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConcaveMeshShape(const ConcaveMeshShape&) = delete;
    ConcaveMeshShape& operator=(const ConcaveMeshShape&) = delete;

    ConcaveMeshShape(ConcaveMeshShape&&) = delete;
    ConcaveMeshShape& operator=(ConcaveMeshShape&&) = delete;

    ~ConcaveMeshShape() override = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] int GetTriangleCount() const;

    [[nodiscard]] std::array<Vector3<float>, 3> GetTriangleVertices(int index) const;

    [[nodiscard]] std::array<Vector3<float>, 3> GetTriangleNormals(int index) const;

    [[nodiscard]] std::array<int, 3> GetTriangleIndices(int index) const;

    [[nodiscard]] int ComputeTriangleShapeId(int index) const;

    void GetLocalBounds(Vector3<>& min, Vector3<>& max) const override;

    [[nodiscard]] std::string ToString() const override;

protected:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    explicit ConcaveMeshShape(std::shared_ptr<Mesh> mesh, const Vector3<float>& scaling = Vector3<float>::One);

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::shared_ptr<Mesh> mesh_;

    DynamicAABBTree dynamicAABBTree_;

    std::vector<Vector3<float>> computedVerticesNormals_{};


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    bool Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const override;

    [[nodiscard]] size_t Size() const override;

    void InitBHVTree();

    void ComputeOverlappingTriangles(const AABB& localAABB, std::vector<Vector3<>>& triangleVertices,
                                     std::vector<Vector3<>>& triangleVertexNormals,
                                     std::vector<int>& shapeIds) const override;

    void ComputeNormals();
};


inline void BlocksEngine::ConvexTriangleAABBOverlapCallback::NotifyOverlappingNode(const int nodeId)
{
    const int32_t* data = dynamicAABBTree_.GetNodeDataInt(nodeId);

    const std::array<Vector3<float>, 3> trianglePoints = concaveMeshShape_.GetTriangleVertices(data[1]);
    const std::array<Vector3<float>, 3> verticesNormals = concaveMeshShape_.GetTriangleNormals(data[1]);

    triangleTestCallback_.TestTriangle(trianglePoints, verticesNormals,
                                       concaveMeshShape_.ComputeTriangleShapeId(data[1]));
}

inline bool BlocksEngine::ConcaveMeshRaycastCallback::IsHit() const
{
    return isHit_;
}

inline void BlocksEngine::ConcaveMeshShape::GetLocalBounds(Vector3<>& min, Vector3<>& max) const
{
    const AABB aabb = dynamicAABBTree_.GetRootAABB();

    min = aabb.GetMin();
    max = aabb.GetMax();
}

inline size_t BlocksEngine::ConcaveMeshShape::Size() const
{
    return sizeof ConcaveMeshShape;
}
