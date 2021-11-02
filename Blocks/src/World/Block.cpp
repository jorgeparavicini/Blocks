#include "Blocks/pch.h"
#include "Blocks/Block.h"

Blocks::Block::Block(const uint8_t id, std::array<std::array<float, 2>, 6> textures, const bool isSeeThrough) noexcept
    : id_{id},
      textures_{textures},
      isSeeThrough_{isSeeThrough}
{
}

uint8_t Blocks::Block::GetId() const noexcept
{
    return id_;
}

const std::array<std::array<float, 2>, 6>& Blocks::Block::GetTextures() const noexcept
{
    return textures_;
}

bool Blocks::Block::IsSeeThrough() const noexcept
{
    return isSeeThrough_;
}

const Blocks::Block Blocks::Block::Air = {
    0, {
        {
            // Front
            {3.0f, 0.0f},
            // Left
            {3.0f, 0.0f},
            // Back
            {3.0f, 0.0f},
            // Right
            {3.0f, 0.0f},
            // Top
            {0.0f, 0.0f},
            // Bottom
            {2.0f, 0.0f}
        }
    },
    true
};

const Blocks::Block Blocks::Block::Dirt = {
    1,
    {
        {
            // Front
            {3.0f, 0.0f},
            // Left
            {3.0f, 0.0f},
            // Back
            {3.0f, 0.0f},
            // Right
            {3.0f, 0.0f},
            // Top
            {0.0f, 0.0f},
            // Bottom
            {2.0f, 0.0f}
        }
    },
    false
};
