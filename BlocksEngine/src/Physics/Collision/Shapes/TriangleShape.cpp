#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/TriangleShape.h"

#include "BlocksEngine/Core/Transform.h"

using namespace BlocksEngine;

TriangleShape::TriangleShape(const Vector3<>* vertices, const Vector3<>* vertexNormals, const int shapeId)
    : ConvexPolyhedronShape{CollisionShapeName::Triangle},
      faces_{HalfEdgeStructure::Face{}, HalfEdgeStructure::Face{}}
{
    points_[0] = vertices[0];
    points_[1] = vertices[1];
    points_[2] = vertices[2];

    // Compute the triangle normal
    normal_ = (vertices[1] - vertices[0]).Cross(vertices[2] - vertices[0]);
    normal_.Normalize();

    vertexNormals_[0] = vertexNormals[0];
    vertexNormals_[1] = vertexNormals[1];
    vertexNormals_[2] = vertexNormals[2];

    // Faces
    faces_[0].faceVertices.reserve(3);
    faces_[0].faceVertices.push_back(0);
    faces_[0].faceVertices.push_back(1);
    faces_[0].faceVertices.push_back(2);
    faces_[0].edgeIndex = 0;

    faces_[1].faceVertices.reserve(3);
    faces_[1].faceVertices.push_back(0);
    faces_[1].faceVertices.push_back(2);
    faces_[1].faceVertices.push_back(1);
    faces_[1].edgeIndex = 1;

    // Edges
    for (int i = 0; i < 6; i++)
    {
        switch (i)
        {
        case 0:
            edges_[0].vertexIndex = 0;
            edges_[0].twinEdgeIndex = 1;
            edges_[0].faceIndex = 0;
            edges_[0].nextEdgeIndex = 2;
            break;
        case 1:
            edges_[1].vertexIndex = 1;
            edges_[1].twinEdgeIndex = 0;
            edges_[1].faceIndex = 1;
            edges_[1].nextEdgeIndex = 5;
            break;
        case 2:
            edges_[2].vertexIndex = 1;
            edges_[2].twinEdgeIndex = 3;
            edges_[2].faceIndex = 0;
            edges_[2].nextEdgeIndex = 4;
            break;
        case 3:
            edges_[3].vertexIndex = 2;
            edges_[3].twinEdgeIndex = 2;
            edges_[3].faceIndex = 1;
            edges_[3].nextEdgeIndex = 1;
            break;
        case 4:
            edges_[4].vertexIndex = 2;
            edges_[4].twinEdgeIndex = 5;
            edges_[4].faceIndex = 0;
            edges_[4].nextEdgeIndex = 0;
            break;
        case 5:
            edges_[5].vertexIndex = 0;
            edges_[5].twinEdgeIndex = 4;
            edges_[5].faceIndex = 1;
            edges_[5].nextEdgeIndex = 3;
            break;

        default:
            throw std::out_of_range("Index is out of range");
        }
    }


    raycastTestType_ = TriangleRaycastSide::Front;

    id_ = shapeId;
}

void TriangleShape::ComputeSmoothTriangleMeshContact(const CollisionShape& shape1,
                                                     const CollisionShape& shape2,
                                                     const Vector3<>& localContactPointShape1,
                                                     const Vector3<>& localContactPointShape2,
                                                     const Transform& shape1ToWorld,
                                                     const Transform& shape2ToWorld,
                                                     const float penetrationDepth,
                                                     const Vector3<>& outSmoothVertexNormal)
{
    assert(shape1.GetName() != CollisionShapeName::Triangle || shape2.GetName() != CollisionShapeName::Triangle);

    const bool isShape1Triangle = shape1.GetName() == CollisionShapeName::Triangle;
    if (isShape1Triangle || shape2.GetName() == CollisionShapeName::Triangle)
    {
        const TriangleShape& triangleShape = isShape1Triangle
                                                 ? dynamic_cast<const TriangleShape&>(shape1)
                                                 : dynamic_cast<const TriangleShape&>(shape2);

        triangleShape.ComputeSmoothMeshContact(isShape1Triangle ? localContactPointShape1 : localContactPointShape2,
                                               isShape1Triangle ? shape1ToWorld : shape2ToWorld,
                                               isShape1Triangle
                                                   ? shape2ToWorld.GetInverse()
                                                   : shape1ToWorld.GetInverse(),
                                               penetrationDepth,
                                               isShape1Triangle,
                                               isShape1Triangle ? localContactPointShape2 : localContactPointShape1,
                                               outSmoothVertexNormal);
    }
}

void TriangleShape::ComputeSmoothMeshContact(const Vector3<> localContactPointTriangle,
                                             const Transform& triangleShapeToWorldTransform,
                                             const Transform& worldToOtherShapeTransform,
                                             const float penetrationDepth,
                                             const bool isTriangleShape1,
                                             Vector3<> outNewLocalContactPointOtherShape,
                                             Vector3<> outSmoothWorldContactTriangleNormal) const
{
    Vector3 triangleLocalNormal = ComputeSmoothLocalContactNormalForTriangle(localContactPointTriangle);
    Vector3 triangleWorldNormal = triangleShapeToWorldTransform.GetRotation() * triangleLocalNormal;
    const Vector3 triangleToOtherShapePenAxis = isTriangleShape1
                                                    ? outSmoothWorldContactTriangleNormal
                                                    : -outSmoothWorldContactTriangleNormal;

    if (triangleWorldNormal.Dot(triangleToOtherShapePenAxis) < 0.0f)
    {
        triangleWorldNormal = -triangleWorldNormal;
        triangleLocalNormal = -triangleLocalNormal;
    }

    outSmoothWorldContactTriangleNormal = isTriangleShape1 ? triangleWorldNormal : -triangleWorldNormal;

    const Vector3 otherShapePointTriangleSpace = localContactPointTriangle - triangleLocalNormal * penetrationDepth;
    const Vector3 otherShapePoint = worldToOtherShapeTransform * triangleShapeToWorldTransform *
        otherShapePointTriangleSpace;
    outNewLocalContactPointOtherShape = otherShapePoint;
}

Vector3<> TriangleShape::ComputeSmoothLocalContactNormalForTriangle(const Vector3<>& localContactPoint) const
{
    float u, v, w;
}
