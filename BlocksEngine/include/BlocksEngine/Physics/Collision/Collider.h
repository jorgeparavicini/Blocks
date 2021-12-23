// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Collider.h

#pragma once

#include "BlocksEngine/Physics/Body/CollisionBody.h"
#include "BlocksEngine/Physics/Collision/Shapes/CollisionShape.h"
#include "BlocksEngine/Physics/Engine/PhysicsMaterial.h"
#include "Shapes/AABB.h"

namespace BlocksEngine
{
    class Collider;
}

class BlocksEngine::Collider final : Component
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    explicit Collider(std::shared_ptr<CollisionBody> body);

    Collider(const Collider&) = delete;
    Collider& operator=(const Collider&) = delete;

    Collider(Collider&&) = delete;
    Collider& operator=(Collider&&) = delete;

    ~Collider() override;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] std::shared_ptr<CollisionShape> GetCollisionShape();

    // TODO: Could this be a reference?
    [[nodiscard]] std::shared_ptr<const CollisionShape> GetCollisionShape() const;

    [[nodiscard]] std::shared_ptr<CollisionBody> GetBody();

    [[nodiscard]] std::shared_ptr<const CollisionBody> GetBody() const;


    // TODO: I really don't like void pointers
    [[nodiscard]] void* GetUserData() const;

    void SetUserData(void* userData);

    // TODO: These names need to change
    [[nodiscard]] const Transform& GetLocalToBodyTransform() const;

    void SetLocalToBodyTransform(const Transform& transform);

    [[nodiscard]] const Transform GetLocalToWorldTransform() const;

    [[nodiscard]] const AABB GetWorldAABB() const;

    [[nodiscard]] bool TestAABBOverlap(const AABB& worldAABB) const;

    [[nodiscard]] bool TestPointInside(const Vector3<float>& worldPoint) const;

    bool Raycast(const Ray& ray, RaycastInfo& raycastInfo);

    [[nodiscard]] unsigned short GetCollideWithMaskBits() const;

    void SetCollideWithMaskBits(unsigned short collideWithMaskBits);

    [[nodiscard]] unsigned short GetCollisionCategoryBits() const;

    void SetCollisionCategoryBits(unsigned short collisionCategoryBits);

    [[nodiscard]] int GetBroadPhaseId() const;

    // TODO: probably shouldn't return a reference here. Maybe smart pointer?
    PhysicsMaterial& GetMaterial();

    void SetMaterial(const PhysicsMaterial& material);

    [[nodiscard]] bool GetIsTrigger() const;

    void SetIsTrigger(bool isTrigger) const;


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::shared_ptr<CollisionBody> body_;
    PhysicsMaterial material_;

    void* userData_;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void SetHasCollisionShapeChangedSize(bool hasCollisionShapeChangedSize);
};

inline std::shared_ptr<BlocksEngine::CollisionBody> BlocksEngine::Collider::GetBody()
{
    return body_;
}

inline std::shared_ptr<const BlocksEngine::CollisionBody> BlocksEngine::Collider::GetBody() const
{
    return body_;
}

inline void* BlocksEngine::Collider::GetUserData() const
{
    return userData_;
}

inline void BlocksEngine::Collider::SetUserData(void* userData)
{
    userData_ = userData;
}

inline bool BlocksEngine::Collider::TestAABBOverlap(const AABB& worldAABB) const
{
    return worldAABB.TestCollision(GetWorldAABB());
}

inline BlocksEngine::PhysicsMaterial& BlocksEngine::Collider::GetMaterial()
{
    return material_;
}
