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
    static constexpr unsigned long long Size = Width * Depth * Height;

    Chunk(std::weak_ptr<BlocksEngine::Actor> actor, const World& world,
          ChunkCoords coords = ChunkCoords::Zero);

    //------------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& out, const Chunk& chunk);

    [[nodiscard]] const Block& GetWorldBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const Block& GetLocalBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const World& GetWorld() const noexcept;
    [[nodiscard]] ChunkCoords GetCoords() const noexcept;
    [[nodiscard]] bool IsInitialized() const noexcept;

    void SetBlocks(std::vector<uint8_t> blocks);
    void RegenerateMesh(std::function<void()> callback) const;

    [[nodiscard]] static int GetFlatIndex(BlocksEngine::Vector3<int> position);
    [[nodiscard]] static int GetFlatIndex(int x, int y, int z);


private:
    std::optional<std::vector<uint8_t>> blocks_;
    const World& world_;
    const ChunkCoords coords_;
    //BlocksEngine::Mesh mesh_;
};
