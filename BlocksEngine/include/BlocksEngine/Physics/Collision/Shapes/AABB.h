// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: AABB.h

#pragma once
#include "BlocksEngine/Core/Math/Ray.h"

// TODO: Documentation & License

namespace BlocksEngine
{
    class AABB;
}

class BlocksEngine::AABB
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    AABB() = default;

    AABB(const Vector3<float>& minCoordinates, const Vector3<float>& maxCoordinates);

    AABB(const AABB& aabb);
    AABB& operator=(const AABB& aabb);

    AABB(AABB&& aabb) noexcept;
    AABB& operator=(AABB&& aabb) noexcept;

    ~AABB() = default;


    //------------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------------


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] Vector3<float> GetCenter() const;

    [[nodiscard]] const Vector3<float>& GetMin() const;

    void SetMin(const Vector3<float>& min);

    [[nodiscard]] const Vector3<float>& GetMax() const;

    void SetMax(const Vector3<float>& max);

    void Scale(const Vector3<float>& scale);

    [[nodiscard]] Vector3<float> GetExtent() const;

    void Inflate(float dx, float dy, float dz);

    [[nodiscard]] bool TestCollision(const AABB& aabb) const;

    [[nodiscard]] float GetVolume() const;

    void Merge(const AABB& aabb);

    [[nodiscard]] bool Contains(const AABB& aabb) const;

    [[nodiscard]] bool Contains(const Vector3<float>& point) const;

    [[nodiscard]] bool TestCollisionTriangleAABB(const Vector3<float>* trianglePoints) const;

    [[nodiscard]] bool TestRayIntersect(const Ray& ray) const;


    //------------------------------------------------------------------------------
    // Static Methods
    //------------------------------------------------------------------------------

    static AABB CreateFromTriangle(const std::array<Vector3<float>, 3>& trianglePoints);

    static AABB Merge(const AABB& aabb1, const AABB& aabb2);

private:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    Vector3<float> minCoordinates_;
    Vector3<float> maxCoordinates_;
};

inline BlocksEngine::Vector3<> BlocksEngine::AABB::GetCenter() const
{
    return (minCoordinates_ + maxCoordinates_) * 0.5f;
}

inline const BlocksEngine::Vector3<float>& BlocksEngine::AABB::GetMin() const
{
    return minCoordinates_;
}

inline void BlocksEngine::AABB::SetMin(const Vector3<float>& min)
{
    minCoordinates_ = min;
}

inline const BlocksEngine::Vector3<float>& BlocksEngine::AABB::GetMax() const
{
    return maxCoordinates_;
}

inline void BlocksEngine::AABB::SetMax(const Vector3<>& max)
{
    maxCoordinates_ = max;
}

inline void BlocksEngine::AABB::Scale(const Vector3<>& scale)
{
    minCoordinates_ *= scale;
    maxCoordinates_ *= scale;
}

inline BlocksEngine::Vector3<> BlocksEngine::AABB::GetExtent() const
{
    return maxCoordinates_ - minCoordinates_;
}

inline void BlocksEngine::AABB::Inflate(const float dx, const float dy, const float dz)
{
    const Vector3 v = {dx, dy, dz};
    maxCoordinates_ += v;
    minCoordinates_ -= v;
}

inline bool BlocksEngine::AABB::TestCollision(const AABB& aabb) const
{
    if (maxCoordinates_.x < aabb.minCoordinates_.x ||
        aabb.maxCoordinates_.x < minCoordinates_.x)
    {
        return false;
    }
    if (maxCoordinates_.y < aabb.minCoordinates_.y ||
        aabb.maxCoordinates_.y < minCoordinates_.y)
    {
        return false;
    }
    if (maxCoordinates_.z < aabb.minCoordinates_.z ||
        aabb.maxCoordinates_.z < minCoordinates_.z)
    {
        return false;
    }

    return true;
}

inline float BlocksEngine::AABB::GetVolume() const
{
    const Vector3 diff = maxCoordinates_ - minCoordinates_;
    return diff.x * diff.y * diff.z;
}

inline bool BlocksEngine::AABB::TestCollisionTriangleAABB(const Vector3<>* trianglePoints) const
{
    if (Math::Min(trianglePoints[0].x, trianglePoints[1].x, trianglePoints[2].x) > maxCoordinates_.x) return false;
    if (Math::Min(trianglePoints[0].y, trianglePoints[1].y, trianglePoints[2].y) > maxCoordinates_.y) return false;
    if (Math::Min(trianglePoints[0].z, trianglePoints[1].z, trianglePoints[2].z) > maxCoordinates_.z) return false;

    if (Math::Max(trianglePoints[0].x, trianglePoints[1].x, trianglePoints[2].x) < minCoordinates_.x) return false;
    if (Math::Max(trianglePoints[0].y, trianglePoints[1].y, trianglePoints[2].y) < minCoordinates_.y) return false;
    if (Math::Max(trianglePoints[0].z, trianglePoints[1].z, trianglePoints[2].z) < minCoordinates_.z) return false;

    return true;
}

inline bool BlocksEngine::AABB::Contains(const Vector3<>& point) const
{
    return point.x >= minCoordinates_.x - Math::EPSILON && point.x <= maxCoordinates_.x + Math::EPSILON &&
        point.y >= minCoordinates_.y - Math::EPSILON && point.y <= maxCoordinates_.y + Math::EPSILON &&
        point.z >= minCoordinates_.z - Math::EPSILON && point.z <= maxCoordinates_.z + Math::EPSILON;
}

inline BlocksEngine::AABB& BlocksEngine::AABB::operator=(const AABB& aabb)
{
    if (this != &aabb)
    {
        minCoordinates_ = aabb.minCoordinates_;
        maxCoordinates_ = aabb.maxCoordinates_;
    }
    return *this;
}

inline BlocksEngine::AABB& BlocksEngine::AABB::operator=(AABB&& aabb) noexcept
{
    if (this != &aabb)
    {
        minCoordinates_ = std::exchange(aabb.minCoordinates_, Vector3<float>::Zero);
        maxCoordinates_ = std::exchange(aabb.maxCoordinates_, Vector3<float>::Zero);
    }

    return *this;
}
