#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Entity.h"

using namespace BlocksEngine;

constexpr uint32_t Entity::INDEX_BITS = 24;
constexpr uint32_t Entity::INDEX_MASK = (1 << INDEX_BITS) - 1;
constexpr uint32_t Entity::GENERATION_BITS = 8;
constexpr uint32_t Entity::GENERATION_MASK = (1 << GENERATION_BITS) - 1;
constexpr uint32_t Entity::MINIMUM_FREE_INDICES = 1024;

Entity::Entity()
    : id_{0}
{
}

Entity::Entity(uint32_t index, uint32_t generation)
    : id_{index & INDEX_MASK | (generation & GENERATION_MASK) << INDEX_BITS}
{
}

void Entity::SetId(uint32_t index, uint32_t generation)
{
    id_ = index & INDEX_MASK | (generation & GENERATION_MASK) << INDEX_BITS;
}
