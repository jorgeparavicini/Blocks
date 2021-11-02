#include "Blocks/pch.h"
#include "Blocks/BlockRegistry.h"

const std::unordered_map<uint8_t, const Blocks::Block&>& Blocks::BlockRegistry::Blocks()
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
            {Block::Dirt.GetId(), Block::Dirt}
        }
    }
{
}
