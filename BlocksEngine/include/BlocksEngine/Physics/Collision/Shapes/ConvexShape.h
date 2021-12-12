// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConvexShape.h

#pragma once
#include "CollisionShape.h"

namespace BlocksEngine
{
    class ConvexShape;
}

class BlocksEngine::ConvexShape : public CollisionShape
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    ConvexShape(CollisionShapeType type, CollisionShapeName name, float margin = 0.0f);

    ConvexShape(const ConvexShape&) = delete;
    ConvexShape& operator=(const ConvexShape&) = delete;

    ConvexShape(const ConvexShape&&) = delete;
    ConvexShape& operator=(const ConvexShape&&) = delete;

    ~ConvexShape() override = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] float GetMargin() const;

    [[nodiscard]] bool IsConvex() const override;


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    /**
     * \brief Margin used for the GJK collision detection algorithm
     */
    float margin_;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    /**
     * \brief Compute a local support point in a given direction with the object margin
     * \param direction TODO???
     * \return TODO???
     */
    [[nodiscard]] Vector3<float> GetLocalSupportPointWithMargin(const Vector3<float>& direction) const;

    /**
     * \brief Compute a local support point in a given direction without the object margin
     * \param direction 
     * \return 
     */
    [[nodiscard]] virtual Vector3<float> GetLocalSupportPointWithoutMargin(const Vector3<float>& direction) const = 0;
};

inline bool BlocksEngine::ConvexShape::IsConvex() const
{
    return true;
}

inline float BlocksEngine::ConvexShape::GetMargin() const
{
    return margin_;
}
