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

// TODO: Find a better way to keep all blocks referenced
const std::unordered_map<uint8_t, const Blocks::Block&> Blocks::Block::Blocks = {{Dirt.GetId(), Dirt}};

// TODO: Block 0 should be air
const Blocks::Block Blocks::Block::Dirt = {
    1,
    {
        {
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f}
        }
    },
    false
};
