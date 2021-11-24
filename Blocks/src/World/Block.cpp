#include "Blocks/pch.h"
#include "Blocks/World/Block.h"

Blocks::Block::Block(const uint8_t id, std::array<uint8_t, 6> textures, const bool isSeeThrough) noexcept
    : id_{id},
      textures_{textures},
      isSeeThrough_{isSeeThrough}
{
}

uint8_t Blocks::Block::GetId() const noexcept
{
    return id_;
}

const std::array<uint8_t, 6>& Blocks::Block::GetTextures() const noexcept
{
    return textures_;
}

bool Blocks::Block::IsSeeThrough() const noexcept
{
    return isSeeThrough_;
}

const Blocks::Block Blocks::Block::Air = {
    0, {{0, 0, 0, 0, 0, 0}}, true
};

const Blocks::Block Blocks::Block::Dirt = {
    1,
    {
        {
            // Front
            1,
            // Left
            1,
            // Back
            1,
            // Right
            1,
            // Top
            1,
            // Bottom
            1
        }
    },
    false
};

const Blocks::Block Blocks::Block::Grass = {
    2,
    {
        {
            // Front
            0,
            // Left
            0,
            // Back
            0,
            // Right
            0,
            // Top
            2,
            // Bottom
            1
        }
    },
    false
};
