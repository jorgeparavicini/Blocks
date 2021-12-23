// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: TriangleShape.h

#pragma once
#include "ConvexPolyhedronShape.h"

namespace BlocksEngine
{
    enum class TriangleRaycastSide;
    class TriangleShape;
}

enum class BlocksEngine::TriangleRaycastSide
{
    Front,
    Back,
    FrontAndBack
};

class BlocksEngine::TriangleShape final : public ConvexPolyhedronShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------
    TriangleShape(const std::array<Vector3<float>, 3>& vertices,
                  const std::array<Vector3<float>, 3>& vertexNormals,
                  int shapeId);

    TriangleShape(const TriangleShape&) = delete;
    TriangleShape& operator=(const TriangleShape&) = delete;

    TriangleShape(TriangleShape&&) = delete;
    TriangleShape& operator=(TriangleShape&&) = delete;

    ~TriangleShape() override = default;


    //------------------------------------------------------------------------------
    // Methods 
    //------------------------------------------------------------------------------

    void GetLocalBounds(Vector3<>& min, Vector3<>& max) const override;

    void ComputeAABB(AABB& aabb, const Transform& transform) const override;

    [[nodiscard]] TriangleRaycastSide GetRaycastTestType() const;

    void SetRaycastTestType(TriangleRaycastSide testType);

    [[nodiscard]] int GetFaceCount() const override;

    [[nodiscard]] const HalfEdgeStructure::Face& GetFace(int faceIndex) const override;

    [[nodiscard]] int GetVertexCount() const override;

    [[nodiscard]] HalfEdgeStructure::Vertex GetVertex(int vertexIndex) const override;

    [[nodiscard]] Vector3<> GetVertexPosition(int vertexIndex) const override;

    [[nodiscard]] Vector3<> GetFaceNormal(int faceIndex) const override;

    [[nodiscard]] int GetHalfEdgesCount() const override;

    [[nodiscard]] const HalfEdgeStructure::Edge& GetHalfEdge(int edgeIndex) const override;

    [[nodiscard]] Vector3<> GetCentroid() const override;

    [[nodiscard]] float GetVolume() const override;

    bool Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const override;


    static void ComputeSmoothTriangleMeshContact(const CollisionShape& shape1, const CollisionShape& shape2,
                                                 const Vector3<float>& localContactPointShape1,
                                                 const Vector3<float>& localContactPointShape2,
                                                 const Transform& shape1ToWorld, const Transform& shape2ToWorld,
                                                 float penetrationDepth, const Vector3<float>& outSmoothVertexNormal);

    [[nodiscard]] std::string ToString() const override;


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    Vector3<float> points_[3];

    Vector3<float> normal_;

    Vector3<float> vertexNormals_[3];

    TriangleRaycastSide raycastTestType_;

    HalfEdgeStructure::Face faces_[2];

    HalfEdgeStructure::Edge edges_[6];


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] Vector3<float> GetLocalSupportPointWithoutMargin(const Vector3<>& direction) const override;

    [[nodiscard]] Vector3<float> ComputeSmoothLocalContactNormalForTriangle(
        const Vector3<float>& localContactPoint) const;

    [[nodiscard]] bool TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const override;

    [[nodiscard]] size_t Size() const override;

    void GenerateId();

    void ComputeSmoothMeshContact(Vector3<float> localContactPointTriangle,
                                  const Transform& triangleShapeToWorldTransform,
                                  const Transform& worldToOtherShapeTransform,
                                  float penetrationDepth,
                                  bool isTriangleShape1,
                                  Vector3<float> outNewLocalContactPointOtherShape,
                                  Vector3<float> outSmoothWorldContactTriangleNormal) const;
};

inline size_t BlocksEngine::TriangleShape::Size() const
{
    return sizeof TriangleShape;
}

inline BlocksEngine::Vector3<> BlocksEngine::TriangleShape::GetLocalSupportPointWithoutMargin(
    const Vector3<>& direction) const
{
    const Vector3 dotProducts{direction.Dot(points_[0]), direction.Dot(points_[1]), direction.Dot(points_[2])};
    return points_[dotProducts.MaxAxis()];
}

inline void BlocksEngine::TriangleShape::GetLocalBounds(Vector3<>& min, Vector3<>& max) const
{
    const Vector3 xAxis{points_[0].x, points_[1].x, points_[2].x};
    const Vector3 yAxis{points_[0].y, points_[1].y, points_[2].y};
    const Vector3 zAxis{points_[0].z, points_[1].z, points_[2].z};

    // TODO: Can this be done with equality?
    min.x = xAxis.Min();
    min.y = yAxis.Min();
    min.z = zAxis.Min();

    max.x = xAxis.Max();
    max.y = yAxis.Max();
    max.z = zAxis.Max();

    const Vector3 margin{margin_, margin_, margin_};
    min -= margin;
    max += margin;
}

inline bool BlocksEngine::TriangleShape::TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const
{
    return false;
}

inline int BlocksEngine::TriangleShape::GetFaceCount() const
{
    return 2;
}

inline const BlocksEngine::HalfEdgeStructure::Face& BlocksEngine::TriangleShape::GetFace(const int faceIndex) const
{
    assert(faceIndex < 2);
    return faces_[faceIndex];
}

inline int BlocksEngine::TriangleShape::GetVertexCount() const
{
    return 3;
}

inline BlocksEngine::HalfEdgeStructure::Vertex BlocksEngine::TriangleShape::GetVertex(const int vertexIndex) const
{
    assert(vertexIndex < 3);

    // TODO: Convert to int
    HalfEdgeStructure::Vertex vertex{static_cast<unsigned int>(vertexIndex)};
    switch (vertexIndex)
    {
    case 0: vertex.edgeIndex = 0;
        break;
    case 1: vertex.edgeIndex = 1;
        break;
    case 2: vertex.edgeIndex = 2;
        break;
    default:
        throw std::out_of_range("The index is out of range");
    }

    return vertex;
}

inline const BlocksEngine::HalfEdgeStructure::Edge& BlocksEngine::TriangleShape::GetHalfEdge(const int edgeIndex) const
{
    assert(edgeIndex < GetHalfEdgesCount());
    return edges_[edgeIndex];
}

inline BlocksEngine::Vector3<> BlocksEngine::TriangleShape::GetVertexPosition(const int vertexIndex) const
{
    assert(vertexIndex < 3);
    return points_[vertexIndex];
}

inline BlocksEngine::Vector3<> BlocksEngine::TriangleShape::GetFaceNormal(const int faceIndex) const
{
    assert(faceIndex < 2);
    return faceIndex == 0 ? normal_ : -normal_;
}

inline BlocksEngine::Vector3<> BlocksEngine::TriangleShape::GetCentroid() const
{
    return Vector3{points_[0] + points_[1] + points_[2]} / 3.0f;
}

inline int BlocksEngine::TriangleShape::GetHalfEdgesCount() const
{
    return 6;
}

inline BlocksEngine::TriangleRaycastSide BlocksEngine::TriangleShape::GetRaycastTestType() const
{
    return raycastTestType_;
}

inline void BlocksEngine::TriangleShape::SetRaycastTestType(const TriangleRaycastSide testType)
{
    raycastTestType_ = testType;
}

inline std::string BlocksEngine::TriangleShape::ToString() const
{
    return "TriangleShape{v1=" + points_[0].ToString()
        + ", v2=" + points_[1].ToString()
        + ", v3=" + points_[2].ToString()
        + "}";
}

inline float BlocksEngine::TriangleShape::GetVolume() const
{
    return 0.0f;
}
