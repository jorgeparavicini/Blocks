#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/BoxShape.h"

#include "BlocksEngine/Core/Math/Ray.h"
#include "BlocksEngine/Physics/Collision/Collider.h"
#include "BlocksEngine/Physics/Collision/RaycastInfo.h"

using namespace BlocksEngine;

BoxShape::BoxShape(const Vector3<>& halfExtents)
    : ConvexPolyhedronShape{CollisionShapeName::Box},
      halfExtents_{halfExtents},
      halfEdgeStructure_{6, 8, 24}
{
    assert(halfExtents.x > 0.0f);
    assert(halfExtents.y > 0.0f);
    assert(halfExtents.z > 0.0f);

    for (int i = 0; i < 8; ++i)
    {
        halfEdgeStructure_.AddVertex(i);
    }

    std::vector<int> face0(4);
    face0[0] = 0;
    face0[1] = 1;
    face0[2] = 2;
    face0[3] = 3;

    std::vector<int> face1(4);
    face1[0] = 1;
    face1[1] = 5;
    face1[2] = 6;
    face1[3] = 2;

    std::vector<int> face2(4);
    face2[0] = 4;
    face2[1] = 7;
    face2[2] = 6;
    face2[3] = 5;

    std::vector<int> face3(4);
    face3[0] = 4;
    face3[1] = 0;
    face3[2] = 3;
    face3[3] = 7;

    std::vector<int> face4(4);
    face4[0] = 4;
    face4[1] = 5;
    face4[2] = 1;
    face4[3] = 0;

    std::vector<int> face5(4);
    face5[0] = 2;
    face5[1] = 6;
    face5[2] = 7;
    face5[3] = 3;

    halfEdgeStructure_.Init();
}

bool BoxShape::Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const
{
    const Vector3<float> rayDirection = ray.point2 - ray.point1;
    float min = std::numeric_limits<float>::min();
    float max = std::numeric_limits<float>::max();

    Vector3<float> normalDirection = Vector3<float>::Zero;
    Vector3<float> currentNormal;

    for (int i = 0; i < 3; ++i)
    {
        if (std::abs(rayDirection[i]) < Math::EPSILON)
        {
            if (ray.point1[i] > halfExtents_[i] || ray.point1[i] < -halfExtents_[i]) return false;
        }
        else
        {
            const float oneOverD = 1.0f / rayDirection[i];
            float t1 = (-halfExtents_[i] - ray.point1[i]) * oneOverD;
            float t2 = (halfExtents_[i] - ray.point1[i]) * oneOverD;

            currentNormal[0] = i == 0 ? -halfExtents_[i] : 0.0f;
            currentNormal[1] = i == 1 ? -halfExtents_[i] : 0.0f;
            currentNormal[2] = i == 2 ? -halfExtents_[i] : 0.0f;

            if (t1 > t2)
            {
                std::swap(t1, t2);
                currentNormal = -currentNormal;
            }

            if (t1 > min)
            {
                min = t1;
                normalDirection = currentNormal;
            }
            max = std::min(max, t2);

            if (min > ray.maxFraction) return false;

            if (min > max) return false;
        }
    }

    if (min < 0.0f || min > ray.maxFraction) return false;

    const Vector3<float> localHitPoint = ray.point1 + min * rayDirection;

    raycastInfo.body = collider.getBody();
    raycastInfo.collider = collider;
    raycastInfo.hitFraction = min;
    raycastInfo.worldPoint = localHitPoint;
    raycastInfo.worldNormal = normalDirection;

    return true;
}
