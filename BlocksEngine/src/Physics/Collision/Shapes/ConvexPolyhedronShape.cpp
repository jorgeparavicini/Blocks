#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConvexPolyhedronShape.h"

BlocksEngine::ConvexPolyhedronShape::ConvexPolyhedronShape(const CollisionShapeName name)
    : ConvexShape{CollisionShapeType::ConvexPolyhedron, name}
{
}

int BlocksEngine::ConvexPolyhedronShape::FindMostAntiParallelFace(const Vector3<>& direction) const
{
    float minDotProduct = std::numeric_limits<float>::max();
    int mostAntiParallelFace = 0;

    for (int i = 0; i < GetFaceCount(); ++i)
    {
        const float dotProduct = GetFaceNormal(i).Dot(direction);
        if (dotProduct < minDotProduct)
        {
            minDotProduct = dotProduct;
            mostAntiParallelFace = i;
        }
    }

    return mostAntiParallelFace;
}
