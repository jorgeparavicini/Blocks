// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: World.h

#pragma once
#include <cstdint>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

#include "Chunk.h"
#include "BlocksEngine/Component.h"
#include "BlocksEngine/Transform.h"

namespace Blocks
{
    class World;
}

class Blocks::World final : public BlocksEngine::Component
{
public:
    using ChunkHash = boost::hash<BlocksEngine::Vector2<int>>;

    World(BlocksEngine::Actor& actor, uint8_t chunkLoadDistance = 8);

    void Update() override;

    void SetPlayerTransform(std::shared_ptr<BlocksEngine::Transform> playerTransform) noexcept;
    [[nodiscard]] Chunk::ChunkCoords ChunkCoordFromPosition(
        const BlocksEngine::Vector3<float>& position) const noexcept;
    [[nodiscard]] const Block& GetBlock(BlocksEngine::Vector3<int> position) const noexcept;

private:
    uint8_t chunkViewDistance_;
    std::unordered_map<Chunk::ChunkCoords, std::shared_ptr<Chunk>, ChunkHash> chunkMap_;

    std::shared_ptr<BlocksEngine::Transform> playerTransform_;
    Chunk::ChunkCoords lastChunkCoords_{Chunk::ChunkCoords::Zero};

    void GenerateWorld() noexcept;
    void CreateChunk(Chunk::ChunkCoords coords);
};
