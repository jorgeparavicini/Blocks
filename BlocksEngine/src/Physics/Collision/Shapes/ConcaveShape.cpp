#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConcaveShape.h"

using namespace BlocksEngine;

ConcaveShape::ConcaveShape(const CollisionShapeName name, const Vector3<>& scaling)
    : CollisionShape{CollisionShapeType::ConcaveShape, name},
      raycastTestType_{TriangleRaycastSide::Front},
      scale_{scaling}
{
}

float ConcaveShape::GetVolume() const
{
    Vector3<float> minBounds, maxBounds;
    GetLocalBounds(minBounds, maxBounds);

    const Vector3<float> length = maxBounds - minBounds;
    return length.x * length.y * length.z;
}
