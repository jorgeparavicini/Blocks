// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Block.h

#pragma once
#include <array>
#include <cstdint>

namespace Blocks
{
    class Block;
}

class Blocks::Block
{
public:
    Block(uint8_t id, std::array<uint8_t, 6> textures, bool isSeeThrough = false) noexcept;

    [[nodiscard]] uint8_t GetId() const noexcept;
    [[nodiscard]] const std::array<uint8_t, 6>& GetTextures() const noexcept;
    [[nodiscard]] bool IsSeeThrough() const noexcept;

    bool operator==(const Block& block) const;

    bool operator!=(const Block& block) const;


    static const Block Air;
    static const Block Dirt;
    static const Block Grass;
    static const Block Stone;

private:
    uint8_t id_;
    std::array<uint8_t, 6> textures_;
    bool isSeeThrough_;
};
