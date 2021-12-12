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
#include "TriangleShape.h"
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

    TriangleCallback(const TriangleCallback&) = delete;
    TriangleCallback& operator=(const TriangleCallback&) = delete;

    TriangleCallback(TriangleCallback&&) = delete;
    TriangleCallback& operator=(TriangleCallback&&) = delete;

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
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConcaveShape(CollisionShapeName name, const Vector3<float>& scaling);

    ConcaveShape(const ConcaveShape&) = delete;
    ConcaveShape& operator=(const ConcaveShape&) = delete;

    ConcaveShape(ConcaveShape&&) = delete;
    ConcaveShape& operator=(ConcaveShape&&) = delete;

    ~ConcaveShape() override = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] TriangleRaycastSide GetRaycastTestType() const;

    void SetRaycastTestType(TriangleRaycastSide testType);

    [[nodiscard]] const Vector3<float>& GetScale() const;

    void SetScale(const Vector3<float>& scale);

    [[nodiscard]] bool IsConvex() const override;

    [[nodiscard]] bool IsPolyhedron() const override;

    virtual void ComputeOverlappingTriangles(const AABB& localAABB,
                                             std::vector<Vector3<float>>& triangleVertices,
                                             std::vector<Vector3<float>>& triangleVertexNormals,
                                             std::vector<int>& shapeIds) const = 0;

    [[nodiscard]] float GetVolume() const override;


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    TriangleRaycastSide raycastTestType_;

    Vector3<float> scale_;

    [[nodiscard]] bool TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const override;
};

inline bool BlocksEngine::ConcaveShape::IsConvex() const
{
    return false;
}

inline bool BlocksEngine::ConcaveShape::IsPolyhedron() const
{
    return true;
}

inline bool BlocksEngine::ConcaveShape::TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const
{
    return false;
}

inline BlocksEngine::TriangleRaycastSide BlocksEngine::ConcaveShape::GetRaycastTestType() const
{
    return raycastTestType_;
}

inline void BlocksEngine::ConcaveShape::SetRaycastTestType(const TriangleRaycastSide testType)
{
    raycastTestType_ = testType;
}

inline const BlocksEngine::Vector3<>& BlocksEngine::ConcaveShape::GetScale() const
{
    return scale_;
}

inline void BlocksEngine::ConcaveShape::SetScale(const Vector3<>& scale)
{
    scale_ = scale;
    NotifyShapeChanged();
}
