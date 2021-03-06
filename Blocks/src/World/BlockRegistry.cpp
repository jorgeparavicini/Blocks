#include "Blocks/pch.h"
#include "Blocks/World/BlockRegistry.h"

#include "Blocks/World/Block.h"

const Blocks::Block& Blocks::BlockRegistry::GetBlock(const uint8_t blockId)
{
    const auto search = Blocks().find(blockId);
    if (search != Blocks().end())
    {
        return search->second;
    }
    throw; // TODO: Application exception
}

const robin_hood::unordered_map<uint8_t, const Blocks::Block&>& Blocks::BlockRegistry::Blocks()
{
    return Instance().blocks_;
}

Blocks::BlockRegistry& Blocks::BlockRegistry::Instance()
{
    static BlockRegistry instance;
    return instance;
}

Blocks::BlockRegistry::BlockRegistry():
    blocks_{
        {
            {Block::Air.GetId(), Block::Air},
            {Block::Dirt.GetId(), Block::Dirt},
            {Block::Grass.GetId(), Block::Grass},
            {Block::Stone.GetId(), Block::Stone}
        }
    }
{
}
