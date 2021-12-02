// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Entity.h

#pragma once
#include <cstdint>

namespace BlocksEngine
{
    class Entity;
}

class BlocksEngine::Entity
{
public:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    Entity(uint32_t index, uint32_t generation);

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] uint32_t GetIndex() const;
    [[nodiscard]] uint32_t GetGeneration() const;

protected:
    /**
    * \brief Number of bits reserved for the index
    */
    static const uint32_t INDEX_BITS;

    /**
     * \brief Mask for the index part of the id
     */
    static const uint32_t INDEX_MASK;

    /**
     * \brief Number of bits reserved for teh generation number
     */
    static const uint32_t GENERATION_BITS;

    /**
     * \brief Mask for the generation part of the id
     */
    static const uint32_t GENERATION_MASK;

    /**
     * \brief Minimum of free indices in the queue before we reuse one from the queue
     */
    static const uint32_t MINIMUM_FREE_INDICES;

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    uint32_t id_;
};

inline uint32_t BlocksEngine::Entity::GetIndex() const
{
    return id_ & INDEX_MASK;
}

inline uint32_t BlocksEngine::Entity::GetGeneration() const
{
    return id_ >> INDEX_BITS & GENERATION_MASK;
}
