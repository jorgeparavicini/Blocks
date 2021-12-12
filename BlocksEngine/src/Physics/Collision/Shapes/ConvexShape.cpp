#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConvexShape.h"

using namespace BlocksEngine;

ConvexShape::ConvexShape(const CollisionShapeType type, const CollisionShapeName name, const float margin)
    : CollisionShape{type, name}, margin_{margin}
{
}

Vector3<> ConvexShape::GetLocalSupportPointWithMargin(const Vector3<>& direction) const
{
    Vector3 supportPoint = GetLocalSupportPointWithoutMargin(direction);

    if (margin_ != 0.0f)
    {
        Vector3 unitVec = Vector3<float>::Down;
        if (direction.LengthSquared() > Math::EPSILON * Math::EPSILON)
        {
            unitVec.Normalize();
        }

        supportPoint += unitVec * margin_;
    }

    return supportPoint;
}
