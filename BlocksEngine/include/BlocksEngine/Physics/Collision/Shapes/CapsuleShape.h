// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: CapsuleShape.h

#pragma once
#include "ConvexShape.h"

namespace BlocksEngine
{
    class CapsuleShape;
}

class BlocksEngine::CapsuleShape : public ConvexShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    CapsuleShape(const CapsuleShape&) = delete;
    CapsuleShape& operator=(const CapsuleShape&) = delete;

    CapsuleShape(CapsuleShape&&) = delete;
    CapsuleShape& operator=(CapsuleShape&&) = delete;

    ~CapsuleShape() override = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] float GetRadius() const;

    void SetRadius(float radius);

    [[nodiscard]] float GetHeight() const;

    void SetHeight(float height);

    void GetLocalBounds(Vector3<float>& min, Vector3<float>& max) const override;

    [[nodiscard]] float GetVolume() const override;

    [[nodiscard]] bool IsPolyhedron() const override;

    [[nodiscard]] std::string ToString() const override;

protected:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    CapsuleShape(float radius, float height);


    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    float halfHeight_;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] Vector3<float> GetLocalSupportPointWithoutMargin(const Vector3<float>& direction) const override;

    [[nodiscard]] bool TestPointInside(const Vector3<float>& worldPoint, const Collider& collider) const override;

    bool Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const override;

    bool RaycastWithSphereEndCap(const Vector3<float>& point1, const Vector3<float>& point2,
                                 const Vector3<float>& sphereCenter, float maxFraction, Vector3<float>& hitLocalPoint,
                                 float& hitFraction) const;

    [[nodiscard]] size_t Size() const override;
};

inline float BlocksEngine::CapsuleShape::GetRadius() const
{
    return margin_;
}

inline void BlocksEngine::CapsuleShape::SetRadius(const float radius)
{
    assert(radius > 0.0f);
    margin_ = radius;

    NotifyShapeChanged();
}

inline float BlocksEngine::CapsuleShape::GetHeight() const
{
    return halfHeight_ * 2;
}

inline void BlocksEngine::CapsuleShape::SetHeight(const float height)
{
    assert(height > 0.0f);
    halfHeight_ = height * 0.5f;

    NotifyShapeChanged();
}

inline size_t BlocksEngine::CapsuleShape::Size() const
{
    return sizeof CapsuleShape;
}

inline void BlocksEngine::CapsuleShape::GetLocalBounds(Vector3<>& min, Vector3<>& max) const
{
    max.x = margin_;
    max.y = halfHeight_ + margin_;
    max.z = margin_;

    min.x = -max.x;
    min.y = -max.y;
    min.z = min.x;
}

inline float BlocksEngine::CapsuleShape::GetVolume() const
{
    return Math::PI * margin_ * margin_ * (4.0f * margin_ / 3.0f + 2.0f * halfHeight_);
}

inline bool BlocksEngine::CapsuleShape::IsPolyhedron() const
{
    return false;
}

inline BlocksEngine::Vector3<> BlocksEngine::CapsuleShape::GetLocalSupportPointWithoutMargin(
    const Vector3<>& direction) const
{
    const float dotProductTop = halfHeight_ * direction.y;

    const float dotProductBottom = -halfHeight_ * direction.y;

    if (dotProductTop > dotProductBottom)
    {
        return {0.0f, halfHeight_, 0.0f};
    }
    return {0.0f, -halfHeight_, 0.0f};
}

inline std::string BlocksEngine::CapsuleShape::ToString() const
{
    return "CapsuleShape{halfHeight=" + std::to_string(halfHeight_) + ", radius=" + std::to_string(GetRadius()) + "}";
}
