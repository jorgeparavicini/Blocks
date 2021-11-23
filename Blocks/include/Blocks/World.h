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
#include <mutex>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <boost/container_hash/hash.hpp>

#include "Chunk.h"
#include "LoadingScreen.h"
#include "BlocksEngine/Component.h"
#include "BlocksEngine/DispatchWorkGroup.h"
#include "BlocksEngine/Transform.h"

namespace Blocks
{
    class World;
}

class Blocks::World final : public BlocksEngine::Component
{
public:
    using ChunkHash = boost::hash<BlocksEngine::Vector2<int>>;

    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    World(std::weak_ptr<BlocksEngine::Actor> actor, std::weak_ptr<BlocksEngine::Transform> playerTransform,
          uint8_t chunkLoadDistance = 8);

    //------------------------------------------------------------------------------
    // Engine Events
    //------------------------------------------------------------------------------

    void Update() override;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]]
    Chunk::ChunkCoords ChunkCoordFromPosition(const BlocksEngine::Vector3<float>& position) const noexcept;

    [[nodiscard]]
    const Block& GetBlock(BlocksEngine::Vector3<int> position) const noexcept;

    void SetPlayerTransform(std::shared_ptr<BlocksEngine::Transform> playerTransform) noexcept;

private:
    std::mutex genLock_;
    std::mutex meshLock_;

    // TODO: This is currently not a radius but just a square where the value is 2x in every x and y directions.
    // The view radius distance
    uint8_t chunkViewDistance_;
    std::unordered_map<Chunk::ChunkCoords, std::shared_ptr<Chunk>, ChunkHash> chunkMap_{};
    std::unordered_set<Chunk::ChunkCoords, ChunkHash> generatingChunks_{};
    //std::unordered_set<Chunk::ChunkCoords, ChunkHash> meshingChunks_{};
    std::queue<Chunk::ChunkCoords> meshRequestQueue_{};
    int chunkMeshingCount_{};
    bool didLoad_{false};

    // These are probably temporary variables. They track where the player is and whether chunks need to be updated.
    std::weak_ptr<BlocksEngine::Transform> playerTransform_;
    Chunk::ChunkCoords lastChunkCoords_{Chunk::ChunkCoords::Zero};


    /**
     * \brief Generates the world.
     * TODO: This could need some implementation @Sevi
     */
    void GenerateWorld() noexcept;

    /**
    * \brief Creates the chunk actor and moves its transform to the world position of the chunk.
    * Then adds a chunk component and registers that component in the chunk Map.
     *
    * \param coords The Coordinates of the chunk.
    * The coordinates are without taking into account the size of the chunk.
    *
    * \return A shared pointer to the created chunk.
    */
    std::shared_ptr<Chunk> CreateChunk(Chunk::ChunkCoords coords);

    /**
     * \brief Generates all blocks for a given chunk and initializes said chunk.
     * \param chunk The uninitialized Chunk to be initialized.
     * \return A list of all blocks in the chunk
     * TODO: This is the main chunk generation that needs to be implemented @Sevi
     */
    [[nodiscard]] std::vector<uint8_t> GenerateChunk(std::shared_ptr<Chunk> chunk) const;

    void OnWorldGenerated();

    void OnWorldLoaded() const;

    std::shared_ptr<BlocksEngine::DispatchWorkGroup> CreateMeshRequestForChunks(
        std::vector<std::shared_ptr<Chunk>> chunks) const;

    void UpdateChunks();


    // TODO: Stuff that should not be in this class but is because i'm lazy
    std::shared_ptr<LoadingScreen> loadingScreen_;
};
