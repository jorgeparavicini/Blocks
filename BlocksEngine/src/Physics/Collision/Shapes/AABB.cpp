#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/AABB.h"

using namespace BlocksEngine;

AABB::AABB(const Vector3<float>& minCoordinates, const Vector3<float>& maxCoordinates)
    : minCoordinates_{minCoordinates}, maxCoordinates_{maxCoordinates}
{
}

AABB::AABB(const AABB& aabb)
    : minCoordinates_{aabb.minCoordinates_}, maxCoordinates_{aabb.maxCoordinates_}
{
}

AABB::AABB(AABB&& aabb) noexcept
    : minCoordinates_{std::exchange(aabb.minCoordinates_, Vector3<float>::Zero)},
      maxCoordinates_{std::exchange(aabb.maxCoordinates_, Vector3<float>::Zero)}
{
}

void AABB::Merge(const AABB& aabb)
{
    minCoordinates_.x = std::min(minCoordinates_.x, aabb.minCoordinates_.x);
    minCoordinates_.y = std::min(minCoordinates_.y, aabb.minCoordinates_.y);
    minCoordinates_.z = std::min(minCoordinates_.z, aabb.minCoordinates_.z);

    maxCoordinates_.x = std::max(maxCoordinates_.x, aabb.maxCoordinates_.x);
    maxCoordinates_.y = std::max(maxCoordinates_.y, aabb.maxCoordinates_.y);
    maxCoordinates_.z = std::max(maxCoordinates_.z, aabb.maxCoordinates_.z);
}

bool AABB::Contains(const AABB& aabb) const
{
    if (minCoordinates_.x > aabb.minCoordinates_.x) return false;
    if (minCoordinates_.y > aabb.minCoordinates_.y) return false;
    if (minCoordinates_.z > aabb.minCoordinates_.z) return false;

    if (maxCoordinates_.x < aabb.maxCoordinates_.x) return false;
    if (maxCoordinates_.y < aabb.maxCoordinates_.y) return false;
    if (maxCoordinates_.z < aabb.maxCoordinates_.z) return false;

    return true;
}

bool AABB::TestRayIntersect(const Ray& ray) const
{
    const Vector3 point2 = ray.point1 + ray.maxFraction * (ray.point2 - ray.point1);
    const Vector3 e = maxCoordinates_ - minCoordinates_;
    const Vector3 d = point2 - ray.point1;
    const Vector3 m = ray.point1 + point2 - minCoordinates_ - maxCoordinates_;

    float adx = std::abs(d.x);
    if (std::abs(m.x) > e.x + adx) return false;

    float ady = std::abs(d.y);
    if (std::abs(m.y) > e.y + ady) return false;

    float adz = std::abs(d.z);
    if (std::abs(m.z) > e.z + adz) return false;

    adx += Math::EPSILON;
    ady += Math::EPSILON;
    adz += Math::EPSILON;

    if (std::abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;
    if (std::abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;
    if (std::abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;

    return true;
}

AABB AABB::CreateFromTriangle(const std::array<Vector3<>, 3>& trianglePoints)
{
    Vector3 minCoords{trianglePoints[0].x, trianglePoints[0].y, trianglePoints[0].z};
    Vector3 maxCoords{trianglePoints[0].x, trianglePoints[0].y, trianglePoints[0].z};

    if (trianglePoints[1].x < minCoords.x) minCoords.x = trianglePoints[1].x;
    if (trianglePoints[1].y < minCoords.y) minCoords.y = trianglePoints[1].y;
    if (trianglePoints[1].z < minCoords.z) minCoords.z = trianglePoints[1].z;

    if (trianglePoints[2].x < minCoords.x) minCoords.x = trianglePoints[2].x;
    if (trianglePoints[2].y < minCoords.y) minCoords.y = trianglePoints[2].y;
    if (trianglePoints[2].z < minCoords.z) minCoords.z = trianglePoints[2].z;

    if (trianglePoints[1].x > maxCoords.x) maxCoords.x = trianglePoints[1].x;
    if (trianglePoints[1].y > maxCoords.y) maxCoords.y = trianglePoints[1].y;
    if (trianglePoints[1].z > maxCoords.z) maxCoords.z = trianglePoints[1].z;

    if (trianglePoints[2].x > maxCoords.x) maxCoords.x = trianglePoints[2].x;
    if (trianglePoints[2].y > maxCoords.y) maxCoords.y = trianglePoints[2].y;
    if (trianglePoints[2].z > maxCoords.z) maxCoords.z = trianglePoints[2].z;

    return AABB{minCoords, maxCoords};
}

AABB AABB::Merge(const AABB& aabb1, const AABB& aabb2)
{
    Vector3 minCoords;
    Vector3 maxCoords;
    minCoords.x = std::min(aabb1.minCoordinates_.x, aabb2.minCoordinates_.x);
    minCoords.y = std::min(aabb1.minCoordinates_.y, aabb2.minCoordinates_.y);
    minCoords.z = std::min(aabb1.minCoordinates_.z, aabb2.minCoordinates_.z);

    maxCoords.x = std::max(aabb1.maxCoordinates_.x, aabb2.maxCoordinates_.x);
    maxCoords.y = std::max(aabb1.maxCoordinates_.y, aabb2.maxCoordinates_.y);
    maxCoords.z = std::max(aabb1.maxCoordinates_.z, aabb2.maxCoordinates_.z);

    return AABB{minCoords, maxCoords};
}
