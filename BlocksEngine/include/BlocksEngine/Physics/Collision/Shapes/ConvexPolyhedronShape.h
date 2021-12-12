// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConvexPolyhedronShape.h

#pragma once
#include "BlocksEngine/Physics/Collision/HalfEdgeStructure.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConvexShape.h"

namespace BlocksEngine
{
    class ConvexPolyhedronShape;
}

class BlocksEngine::ConvexPolyhedronShape : public ConvexShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConvexPolyhedronShape(CollisionShapeName name);

    ConvexPolyhedronShape(const ConvexPolyhedronShape&) = delete;
    ConvexPolyhedronShape& operator=(const ConvexPolyhedronShape&) = delete;

    ConvexPolyhedronShape(const ConvexPolyhedronShape&&) = delete;
    ConvexPolyhedronShape& operator=(const ConvexPolyhedronShape&&) = delete;

    ~ConvexPolyhedronShape() override = default;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] virtual int GetFaceCount() const = 0;

    [[nodiscard]] virtual const HalfEdgeStructure::Face& GetFace(int faceIndex) const = 0;

    [[nodiscard]] virtual int GetVertexCount() const = 0;

    [[nodiscard]] virtual HalfEdgeStructure::Vertex GetVertex(int vertexIndex) const = 0;

    [[nodiscard]] virtual Vector3<float> GetVertexPosition(int vertexIndex) const = 0;

    [[nodiscard]] virtual Vector3<float> GetFaceNormal(int faceIndex) const = 0;

    [[nodiscard]] virtual int GetHalfEdgesCount() const = 0;

    [[nodiscard]] virtual const HalfEdgeStructure::Edge& GetHalfEdge(int edgeIndex) const = 0;

    [[nodiscard]] bool IsPolyhedron() const override;

    [[nodiscard]] virtual Vector3<float> GetCentroid() const = 0;

    [[nodiscard]] int FindMostAntiParallelFace(const Vector3<float>& direction) const;
};

inline bool BlocksEngine::ConvexPolyhedronShape::IsPolyhedron() const
{
    return true;
}
