#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/CollisionShape.h"

#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Math/Matrix3x3.h"
#include "BlocksEngine/Physics/Collision/Shapes/AABB.h"

using namespace BlocksEngine;

CollisionShape::CollisionShape(const CollisionShapeType type, const CollisionShapeName name)
    : type_{type}, name_{name}, id_{0}
{
}

void CollisionShape::ComputeAABB(AABB& aabb, const Transform& transform) const
{
    Vector3<float> minBounds;
    Vector3<float> maxBounds;
    GetLocalBounds(minBounds, maxBounds);

    const Vector3<float>& translation = transform.GetPosition();
    const Matrix3X3 matrix = Matrix3X3::CreateFromQuaternion(transform.GetRotation());

    const Vector3<float> resultMin;
    const Vector3<float> resultMax;

    // For each of the three axis
    for (int i = 0; i < 3; ++i)
    {
        // Add translation component
        resultMin[i] = translation[i];
        resultMax[i] = translation[i];

        for (int j = 0; j < 3; ++j)
        {
            const float e = matrix[i][j] * minBounds[j];
            const float f = matrix[i][j] * maxBounds[j];

            const float min = e < f ? e : f;
            const float max = e > f ? e : f;

            resultMin[i] += min;
            resultMax[i] += max;
        }
    }

    aabb.SetMin(resultMin);
    aabb.SetMax(resultMax);
}

void CollisionShape::NotifyShapeChanged()
{
    for (int i = 0; i < colliders_.size(); ++i)
    {
        colliders_[i]->setHasCollisionShapeChangedSize(true);
    }
}
