// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: BoxShape.h

#pragma once
#include "ConvexPolyhedronShape.h"
#include "BlocksEngine/Physics/Collision/HalfEdgeStructure.h"

namespace BlocksEngine
{
    class BoxShape;
}

class BlocksEngine::BoxShape : public ConvexPolyhedronShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    BoxShape(const BoxShape&) = delete;
    BoxShape& operator=(const BoxShape&) = delete;

    BoxShape(BoxShape&&) = delete;
    BoxShape& operator=(BoxShape&&) = delete;

    ~BoxShape() override = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] Vector3<float> GetHalfExtents() const;

    void SetHalfExtents(const Vector3<float>& halfExtents);

    void GetLocalBounds(Vector3<float>& min, Vector3<float>& max) const override;

    [[nodiscard]] float GetVolume() const override;

    [[nodiscard]] int GetFaceCount() const override;

    [[nodiscard]] const HalfEdgeStructure::Face& GetFace(int faceIndex) const override;

    [[nodiscard]] int GetVertexCount() const override;

    [[nodiscard]] HalfEdgeStructure::Vertex GetVertex(int vertexIndex) const override;

    [[nodiscard]] int GetHalfEdgesCount() const override;

    [[nodiscard]] const HalfEdgeStructure::Edge& GetHalfEdge(int edgeIndex) const override;

    [[nodiscard]] Vector3<float> GetVertexPosition(int vertexIndex) const override;

    [[nodiscard]] Vector3<float> GetFaceNormal(int faceIndex) const override;

    [[nodiscard]] Vector3<float> GetCentroid() const override;

    [[nodiscard]] std::string ToString() const override;

protected:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    BoxShape(const Vector3<float>& halfExtents);


    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    Vector3<float> halfExtents_;

    HalfEdgeStructure halfEdgeStructure_;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] Vector3<float> GetLocalSupportPointWithoutMargin(const Vector3<float>& direction) const override;

    [[nodiscard]] bool TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const override;

    bool Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const override;

    [[nodiscard]] size_t Size() const override;
};

inline BlocksEngine::Vector3<> BlocksEngine::BoxShape::GetHalfExtents() const
{
    return halfExtents_;
}

inline void BlocksEngine::BoxShape::SetHalfExtents(const Vector3<>& halfExtents)
{
    halfExtents_ = halfExtents;

    NotifyShapeChanged();
}

inline void BlocksEngine::BoxShape::GetLocalBounds(Vector3<>& min, Vector3<>& max) const
{
    max = halfExtents_;
    min = -halfExtents_;
}

inline size_t BlocksEngine::BoxShape::Size() const
{
    return sizeof BoxShape;
}

inline BlocksEngine::Vector3<> BlocksEngine::BoxShape::GetLocalSupportPointWithoutMargin(
    const Vector3<>& direction) const
{
    return {
        direction.x < 0.0f ? -halfExtents_.x : halfExtents_.x,
        direction.y < 0.0f ? -halfExtents_.y : halfExtents_.y,
        direction.z < 0.0f ? -halfExtents_.z : halfExtents_.z
    };
}

inline bool BlocksEngine::BoxShape::TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const
{
    return
        worldPoint.x < halfExtents_[0] && worldPoint.x > -halfExtents_[0] &&
        worldPoint.y < halfExtents_[0] && worldPoint.y > -halfExtents_[1] &&
        worldPoint.z < halfExtents_[0] && worldPoint.z > -halfExtents_[2];
}

inline int BlocksEngine::BoxShape::GetFaceCount() const
{
    return 6;
}

inline const BlocksEngine::HalfEdgeStructure::Face& BlocksEngine::BoxShape::GetFace(const int faceIndex) const
{
    assert(faceIndex < halfEdgeStructure_.GetFaceCount());

    return halfEdgeStructure_.GetFace(faceIndex);
}

inline int BlocksEngine::BoxShape::GetVertexCount() const
{
    return 8;
}

inline BlocksEngine::HalfEdgeStructure::Vertex BlocksEngine::BoxShape::GetVertex(const int vertexIndex) const
{
    assert(vertexIndex < GetVertexCount());
    return halfEdgeStructure_.GetVertex(vertexIndex);
}

inline BlocksEngine::Vector3<> BlocksEngine::BoxShape::GetVertexPosition(const int vertexIndex) const
{
    Vector3<float> extent = GetHalfExtents();

    switch (vertexIndex)
    {
    case 0: return {-extent.x, -extent.y, extent.z}; // NOLINT(bugprone-branch-clone)
    case 1: return {extent.x, -extent.y, extent.z};
    case 2: return {extent.x, extent.y, extent.z};
    case 3: return {-extent.x, extent.y, extent.z};
    case 4: return {-extent.x, -extent.y, -extent.z};
    case 5: return {extent.x, -extent.y, -extent.z};
    case 6: return {extent.x, extent.y, -extent.z};
    case 7: return {-extent.x, extent.y, -extent.z};
    default: throw std::out_of_range("Index was out of range");
    }
}

inline BlocksEngine::Vector3<> BlocksEngine::BoxShape::GetFaceNormal(const int faceIndex) const
{
    switch (faceIndex)
    {
    case 0: return {0, 0, 1}; // NOLINT(bugprone-branch-clone)
    case 1: return {1, 0, 0};
    case 2: return {0, 0, -1};
    case 3: return {-1, 0, 0};
    case 4: return {0, -1, 0};
    case 5: return {0, 1, 0};
    default: throw std::out_of_range("Index was out of range");
    }
}

inline BlocksEngine::Vector3<> BlocksEngine::BoxShape::GetCentroid() const
{
    return Vector3<float>::Zero;
}

inline float BlocksEngine::BoxShape::GetVolume() const
{
    return 8 * halfExtents_.x * halfExtents_.y * halfExtents_.z;
}

inline std::string BlocksEngine::BoxShape::ToString() const
{
    return "BoxShape{extents=" + halfExtents_.ToString() + "}";
}

inline int BlocksEngine::BoxShape::GetHalfEdgesCount() const
{
    return 24;
}

inline const BlocksEngine::HalfEdgeStructure::Edge& BlocksEngine::BoxShape::GetHalfEdge(const int edgeIndex) const
{
    assert(edgeIndex < GetHalfEdgesCount());
    return halfEdgeStructure_.GetHalfEdge(edgeIndex);
}
