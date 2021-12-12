#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/CapsuleShape.h"

#include "BlocksEngine/Core/Math/Ray.h"
#include "BlocksEngine/Physics/Collision/RaycastInfo.h"

using namespace BlocksEngine;

CapsuleShape::CapsuleShape(const float radius, const float height)
    : ConvexShape{CollisionShapeType::Capsule, CollisionShapeName::Capsule, radius},
      halfHeight_{height * 0.5f}
{
    assert(radius > 0.0f);
    assert(height > 0.0f);
}

bool CapsuleShape::TestPointInside(const Vector3<>& worldPoint, const Collider& collider) const
{
    const float diffYCenterSphere1 = worldPoint.y - halfHeight_;
    const float diffYCenterSphere2 = worldPoint.y + halfHeight_;
    const float xSquare = worldPoint.x * worldPoint.x;
    const float zSquare = worldPoint.z * worldPoint.z;
    const float squareRadius = margin_ * margin_;

    return xSquare + zSquare < squareRadius && worldPoint.y < halfHeight_ && worldPoint.y > -halfHeight_ ||
        xSquare + zSquare + diffYCenterSphere1 * diffYCenterSphere1 < squareRadius ||
        xSquare + zSquare + diffYCenterSphere2 * diffYCenterSphere2 < squareRadius;
}

bool CapsuleShape::Raycast(const Ray& ray, RaycastInfo& raycastInfo, const std::shared_ptr<Collider> collider) const
{
    const Vector3<float> n = ray.point2 - ray.point1;

    const Vector3 p{0.0f, -halfHeight_, 0.0f};
    const Vector3 q{0.0f, halfHeight_, 0.0f};
    const Vector3 d = q - p;
    const Vector3 m = ray.point1 - p;
    float t;

    const float mDotD = m.Dot(d);
    const float nDotD = n.Dot(d);
    const float dDotD = n.Dot(d);

    const float vec1DotD = (ray.point1 - Vector3{0.0f, -halfHeight_ - margin_, 0.0f}).Dot(d);
    if (vec1DotD < 0.0f && vec1DotD + nDotD < 0.0f) return false;

    const float dDotDExtraCaps = 2.0f * margin_ * d.y;
    if (vec1DotD > dDotD + dDotDExtraCaps && vec1DotD + nDotD > dDotD + dDotDExtraCaps) return false;

    const float nDotN = n.Dot(n);
    const float mDotN = m.Dot(n);

    const float a = dDotD * nDotN - nDotD * nDotD;
    const float k = m.Dot(m) - margin_ * margin_;
    const float c = dDotD * k - mDotD * mDotD;

    if (std::abs(a) < Math::EPSILON)
    {
        if (c > 0.0f) return false;

        if (mDotD < 0.0f)
        {
            Vector3<float> hitLocalPoint;
            float hitFraction;

            if (RaycastWithSphereEndCap(ray.point1, ray.point2, p, ray.maxFraction, hitLocalPoint, hitFraction))
            {
                raycastInfo.body = collider->GetBody();
                raycastInfo.collider = collider;
                raycastInfo.hitFraction = hitFraction;
                raycastInfo.worldPoint = hitLocalPoint;
                raycastInfo.worldNormal = hitLocalPoint - p;

                return true;
            }

            return false;
        }
        if (mDotD > dDotD)
        {
            Vector3<float> hitLocalPoint;
            float hitFraction;
            if (RaycastWithSphereEndCap(ray.point1, ray.point2, q, ray.maxFraction, hitLocalPoint, hitFraction))
            {
                raycastInfo.body = collider->GetBody();
                raycastInfo.collider = collider;
                raycastInfo.hitFraction = hitFraction;
                raycastInfo.worldPoint = hitLocalPoint;
                raycastInfo.worldNormal = hitLocalPoint - q;

                return true;
            }

            return false;
        }
        return false;
    }

    const float b = dDotD * mDotN - nDotD * mDotD;
    const float discriminant = b * b - a * c;

    if (discriminant < 0.0f) return false;

    const float t0 = t = (-b - std::sqrt(discriminant)) / a;

    const float value = mDotD + t * nDotD;
    if (value < 0.0f)
    {
        Vector3<float> hitLocalPoint;
        float hitFraction;
        if (RaycastWithSphereEndCap(ray.point1, ray.point2, p, ray.maxFraction, hitLocalPoint, hitFraction))
        {
            raycastInfo.body = collider->GetBody();
            raycastInfo.collider = collider;
            raycastInfo.hitFraction = hitFraction;
            raycastInfo.worldPoint = hitLocalPoint;
            raycastInfo.worldNormal = hitLocalPoint - p;

            return true;
        }

        return false;
    }
    if (value > dDotD)
    {
        Vector3<float> hitLocalPoint;
        float hitFraction;
        if (RaycastWithSphereEndCap(ray.point1, ray.point2, q, ray.maxFraction, hitLocalPoint, hitFraction))
        {
            raycastInfo.body = collider->GetBody();
            raycastInfo.collider = collider;
            raycastInfo.hitFraction = hitFraction;
            raycastInfo.worldPoint = hitLocalPoint;
            raycastInfo.worldNormal = hitLocalPoint - q;

            return true;
        }

        return false;
    }

    t = t0;

    if (t < 0.0f || t > ray.maxFraction) return false;

    const Vector3 localHitPoint = ray.point1 + t * n;
    raycastInfo.body = collider->GetBody();
    raycastInfo.collider = collider;
    raycastInfo.hitFraction = t;
    raycastInfo.worldPoint = localHitPoint;
    const Vector3 v = localHitPoint - p;
    const Vector3 w = v.Dot(d) / d.LengthSquared() * d;
    Vector3 normalDirection;
    (localHitPoint - (p + w)).Normalize(normalDirection);
    raycastInfo.worldNormal = normalDirection;

    return true;
}

bool CapsuleShape::RaycastWithSphereEndCap(const Vector3<>& point1, const Vector3<>& point2,
                                           const Vector3<>& sphereCenter, float maxFraction, Vector3<>& hitLocalPoint,
                                           float& hitFraction) const
{
    const Vector3 m = point1 - sphereCenter;
    const float c = m.Dot(m) - margin_ * margin_;

    if (c < 0.0f) return false;

    const Vector3 rayDirection = point2 - point1;
    const float b = m.Dot(rayDirection);

    if (b > 0.0f) return false;

    const float raySquareLength = rayDirection.LengthSquared();

    const float discriminant = b * b - raySquareLength * c;

    if (discriminant < 0.0f || raySquareLength < Math::EPSILON) return false;

    float t = -b - std::sqrt(discriminant);

    assert(t >= 0.0f);

    if (t < maxFraction * raySquareLength)
    {
        t /= raySquareLength;
        hitFraction = t;
        hitLocalPoint = point1 + t * rayDirection;

        return true;
    }

    return false;
}
