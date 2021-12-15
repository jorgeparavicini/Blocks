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
#include <FastNoise/FastNoise.h>

#include "Chunk.h"
#include "LoadingScreen.h"
#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkGroup.h"

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

    World(std::weak_ptr<BlocksEngine::Transform> playerTransform, uint8_t chunkLoadDistance = 8);

    //------------------------------------------------------------------------------
    // Engine Events
    //------------------------------------------------------------------------------

    void Start() override;
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
    std::unordered_map<Chunk::ChunkCoords, std::shared_ptr<Chunk>, ChunkHash> chunks_{};
    std::unordered_set<Chunk::ChunkCoords, ChunkHash> activeChunkCoords_{};

    // These are probably temporary variables. They track where the player is and whether chunks need to be updated.
    std::weak_ptr<BlocksEngine::Transform> playerTransform_;
    Chunk::ChunkCoords lastChunkCoords_{Chunk::ChunkCoords::Zero};

    FastNoise::SmartNode<FastNoise::Perlin> fnGenerator_;

    // TODO: We should add signals in order for other subjects to listen to world changes

    /**
     * \brief Generates the world.
     * It first loads all chunks and creates their meshes once all have been generated.
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
    [[nodiscard]] Chunk::ChunkData GenerateChunk(std::shared_ptr<Chunk> chunk) const;

    void OnWorldGenerated();

    void OnWorldLoaded() const;

    /**
     * \brief A chunk generation request is a Dispatch Work Item that is responsible for calling GenerateChunk
     * and assigning the result on the main thread to the chunk
     * \param chunk The chunk to generate the blocks for
     * \return A DispatchWorkItem that generates a chunk once executed.
     */
    [[nodiscard]] std::shared_ptr<BlocksEngine::DispatchWorkItem> CreateGenerationRequestForChunk(
        std::shared_ptr<Chunk> chunk) const;

    [[nodiscard]] std::shared_ptr<BlocksEngine::DispatchWorkGroup> CreateMeshRequestGroup(
        std::vector<std::shared_ptr<Chunk>> chunks) const;


    void UpdateChunks();


    // TODO: Stuff that should not be in this class but is because i'm lazy
    std::shared_ptr<LoadingScreen> loadingScreen_;
};
