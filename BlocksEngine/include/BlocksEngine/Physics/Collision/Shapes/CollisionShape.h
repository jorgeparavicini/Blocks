/********************************************************************************
* ReactPhysics3D physics library, http://www.reactphysics3d.com                 *
* Copyright (c) 2010-2020 Daniel Chappuis                                       *
*********************************************************************************
*                                                                               *
* This software is provided 'as-is', without any express or implied warranty.   *
* In no event will the authors be held liable for any damages arising from the  *
* use of this software.                                                         *
*                                                                               *
* Permission is granted to anyone to use this software for any purpose,         *
* including commercial applications, and to alter it and redistribute it        *
* freely, subject to the following restrictions:                                *
*                                                                               *
* 1. The origin of this software must not be misrepresented; you must not claim *
*    that you wrote the original software. If you use this software in a        *
*    product, an acknowledgment in the product documentation would be           *
*    appreciated but is not required.                                           *
*                                                                               *
* 2. Altered source versions must be plainly marked as such, and must not be    *
*    misrepresented as being the original software.                             *
*                                                                               *
* 3. This notice may not be removed or altered from any source distribution.    *
*                                                                               *
*                                                                               *
* Modified by Jorge Paravicini                                                  *
********************************************************************************/
#pragma once

#include "BlocksEngine/Core/Math/Math.h"

// TODO: Documentation

namespace BlocksEngine
{
    /// Declarations
    class Collider;
    class CollisionBody;
    class AABB;
    class Transform;
    struct Ray;
    struct RaycastInfo;

    class CollisionShape;
}

class BlocksEngine::CollisionShape
{
public:
    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    enum class CollisionShapeType
    {
        Sphere,
        Capsule,
        ConvexPolyhedron,
        ConcaveShape
    };

    enum class CollisionShapeName
    {
        Triangle,
        Sphere,
        Capsule,
        Box,
        ConvexMesh,
        TriangleMesh,
        HeightField
    };


    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    /**
     * \brief Construct a new collision shape
     * \param type The type of the new collision shape
     * \param name The name of the new collision shape
     */
    CollisionShape(CollisionShapeType type, CollisionShapeName name);

    /**
     * \brief Virtual destructor to allow virtual methods
     */
    virtual ~CollisionShape() = default;

    // Deleted methods
    CollisionShape(const CollisionShape&) = delete;
    CollisionShape& operator=(const CollisionShape&) = delete;

    CollisionShape(const CollisionShape&&) = delete;
    CollisionShape& operator=(const CollisionShape&&) = delete;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    /**
     * \brief Get the name of the collision shape
     * \return The name of the collision shape
     */
    [[nodiscard]] CollisionShapeName GetName() const;

    /**
     * \brief Get the type of the collision shape
     * \return The type of the collision shape
     */
    [[nodiscard]] CollisionShapeType GetType() const;

    /**
     * \brief Get the id of the shape
     * \return The id of the shape
     */
    [[nodiscard]] uint32_t GetId() const;

    /**
     * \brief Calculates whether the collision shape is convex or concave
     * \return True if the shape is convex, false if it is concave
     */
    [[nodiscard]] virtual bool IsConvex() const = 0;

    /**
     * \brief Calculate whether the shape is a polyhedron
     * \return True if the shape is a polyhedron
     */
    [[nodiscard]] virtual bool IsPolyhedron() const = 0;

    /**
     * \brief Calculate the local bounds of the shape in x, y and z directions
     * \param min The minimum bounds of the shape
     * \param max The maximum bounds of the shape
     */
    virtual void GetLocalBounds(Vector3<float>& min, Vector3<float>& max) const = 0;

    /**
     * \brief Calculate the volume of the collision shape
     * \return The volume of the collision shape
     */
    [[nodiscard]] virtual float GetVolume() const = 0;

    /**
     * \brief Compute the world-space AABB of the collision shape given a transform
     * \param aabb The resulting world-space aabb
     * \param transform TODO????
     */
    virtual void ComputeAABB(AABB& aabb, const Transform& transform) const;

    /**
     * \brief Create a string representation of the collision shape
     * \return The string representation of the collision shape
     */
    [[nodiscard]] virtual std::string ToString() const = 0;


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    /**
     * \brief Type of the collision shape
     */
    CollisionShapeType type_;

    /**
     * \brief Name of the collision shape
     */
    CollisionShapeName name_;

    /**
     * \brief Unique identifier of the shape inside an overlapping pair
     */
    uint32_t id_;

    /**
     * \brief List of the colliders associated with this shape
     */
    std::vector<std::shared_ptr<Collider>> colliders_{};

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    /**
     * \brief Test whether a given point is inside the collision shape
     * \param worldPoint The point to be tested
     * \param collider //TODO??
     * \return True if the point is inside the collision shape
     */
    [[nodiscard]] virtual bool TestPointInside(const Vector3<float>& worldPoint, const Collider& collider) const = 0;

    /**
     * \brief Raycast method with feedback information
     * \param ray
     * \param raycastInfo
     * \param collider
     * \return
     */
    virtual bool Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const = 0;

    /**
     * \brief Calculates the size of the collision shape
     * \return The number of bytes used by the collision shape
     */
    [[nodiscard]] virtual size_t Size() const = 0;

    /**
     * \brief Add a new collider to the collision shape
     * \param collider The collider to be added to the collision shape
     */
    void AddCollider(std::shared_ptr<Collider> collider);

    /**
     * \brief Remove a given collider from the collision shape
     * \param collider The collider to be removed from the collision shape
     */
    void RemoveCollider(std::shared_ptr<Collider> collider);

    /**
     * \brief Notifies all assigned colliders, that the collision shape has changed
     */
    void NotifyShapeChanged();
};

//------------------------------------------------------------------------------
// Inline
//------------------------------------------------------------------------------

inline BlocksEngine::CollisionShape::CollisionShapeName BlocksEngine::CollisionShape::GetName() const
{
    return name_;
}

inline BlocksEngine::CollisionShape::CollisionShapeType BlocksEngine::CollisionShape::GetType() const
{
    return type_;
}

inline uint32_t BlocksEngine::CollisionShape::GetId() const
{
    return id_;
}

inline void BlocksEngine::CollisionShape::AddCollider(std::shared_ptr<Collider> collider)
{
    colliders_.push_back(std::move(collider));
}

inline void BlocksEngine::CollisionShape::RemoveCollider(std::shared_ptr<Collider> collider)
{
    // TODO: Implement once colliders have hashing
    //colliders_.erase()
}
