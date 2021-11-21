// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: BlockRegistry.h

#pragma once
#include <unordered_map>

#include "Blocks/Block.h"

namespace Blocks
{
    class BlockRegistry;
}

class Blocks::BlockRegistry
{
public:
    BlockRegistry(const BlockRegistry&) = delete;
    void operator=(const BlockRegistry&) = delete;

    BlockRegistry(const BlockRegistry&&) = delete;
    void operator=(const BlockRegistry&&) = delete;

    ~BlockRegistry() = default;

    static const Block& GetBlock(uint8_t blockId);
    static const std::unordered_map<uint8_t, const Block&>& Blocks();

private:
    static BlockRegistry& Instance();
    BlockRegistry();

    std::unordered_map<uint8_t, const Block&> blocks_;
};
