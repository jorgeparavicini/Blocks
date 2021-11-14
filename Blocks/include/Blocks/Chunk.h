// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Chunk.h

#pragma once

#include "Block.h"
#include "BlocksEngine/Component.h"
#include "BlocksEngine/Mesh.h"
#include "BlocksEngine/Vector2.h"

namespace Blocks
{
    class World;

    class Chunk;
}

class Blocks::Chunk final : public BlocksEngine::Component
{
public:
    using ChunkCoords = BlocksEngine::Vector2<int>;

    static constexpr int Width = 16;
    static constexpr int Depth = 16;
    static constexpr int Height = 16;

    Chunk(BlocksEngine::Actor& actor, const World& world,
          ChunkCoords coords = ChunkCoords::Zero);

    [[nodiscard]] const Block& GetWorldBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const Block& GetLocalBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const World& GetWorld() const noexcept;

    void RegenerateMesh() const;


private:
    std::array<uint8_t, static_cast<unsigned long long>(Width * Depth * Height)> blocks_;
    const World& world_;
    const ChunkCoords coords_;
    //BlocksEngine::Mesh mesh_;

    [[nodiscard]] static int GetFlatIndex(BlocksEngine::Vector3<int> position);
    [[nodiscard]] static int GetFlatIndex(int x, int y, int z);
};
