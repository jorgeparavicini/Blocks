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
#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Core/Components/Renderer.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkGroup.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkItem.h"
#include "BlocksEngine/Core/Math/Vector2.h"
#include "BlocksEngine/Graphics/Material/Texture2D.h"
#include "BlocksEngine/Graphics/Material/Terrain/Terrain.h"

// TODO: Split chunks into smaller sections of 16 height

namespace Blocks
{
    class World;

    class Chunk;
}

class Blocks::Chunk final : public BlocksEngine::Component
{
public:
    //------------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------------

    /**
     * \brief The number of blocks in a chunk on the X axis
     */
    static constexpr int Width = 16;

    /**
     * \brief The number of blocks in a chunk on the Z axis
     */
    static constexpr int Depth = 16;

    /**
     * \brief Each chunk is split into multiple sections stacked on top of each other.
     * The SectionHeight indicates the nr of blocks per section on the Y axis
     */
    static constexpr int SectionHeight = 48;

    /**
     * \brief The number of sections to stack on top of each other per chunk
     */
    static constexpr int SectionsPerChunk = 1;

    /**
     * \brief The total number of blocks in a chunk on the Y axis
     */
    static constexpr int Height = SectionHeight * SectionsPerChunk;

    /**
     * \brief The total number of blocks in a chunk
     */
    static constexpr int Size = Width * Depth * Height;


    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    // TODO: Once child actors are supported place each section into a child actor with position (0, y, 0) where y is the section nr * section height
    class ChunkSection final : public Component
    {
    public:
        //------------------------------------------------------------------------------
        // Constructors
        //------------------------------------------------------------------------------

        ChunkSection(const Chunk& chunk, int section);


        //------------------------------------------------------------------------------
        // Deleted Copy & Assignment
        //------------------------------------------------------------------------------

        ChunkSection(const ChunkSection&) = delete;
        ChunkSection& operator=(const ChunkSection&) = delete;

        ChunkSection(const ChunkSection&&) = delete;
        ChunkSection& operator=(const ChunkSection&&) = delete;

        ~ChunkSection() override = default;


        //------------------------------------------------------------------------------
        // Methods
        //------------------------------------------------------------------------------

        void Start() override;
        std::unique_ptr<BlocksEngine::DispatchWorkItem> RegenerateMesh();
        void Enable() noexcept;
        void Disable() noexcept;

    private:
        inline static std::shared_ptr<BlocksEngine::Texture2D> terrainTexture_;

        const Chunk& chunk_;
        const int section_;
        std::shared_ptr<BlocksEngine::Renderer> renderer_;

        [[nodiscard]] const Block& GetBlock(BlocksEngine::Vector3<int> position) const noexcept;
    };


    //------------------------------------------------------------------------------
    // Type definitions
    //------------------------------------------------------------------------------

    using ChunkCoords = BlocksEngine::Vector2<int>;
    using ChunkData = std::vector<uint8_t>;


    //------------------------------------------------------------------------------
    // Constructor
    //------------------------------------------------------------------------------

    Chunk(const World& world, ChunkCoords coords = ChunkCoords::Zero);

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void Enable() noexcept;
    void Disable() noexcept;

    //------------------------------------------------------------------------------
    // Operators
    //------------------------------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& out, const Chunk& chunk);

    void Start() override;
    [[nodiscard]] const Block& GetWorldBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const Block& GetLocalBlock(BlocksEngine::Vector3<int> position) const noexcept;
    [[nodiscard]] const World& GetWorld() const noexcept;
    [[nodiscard]] ChunkCoords GetCoords() const noexcept;
    [[nodiscard]] bool IsInitialized() const noexcept;

    void SetBlocks(ChunkData blocks);
    [[nodiscard]] std::unique_ptr<BlocksEngine::DispatchWorkGroup> RegenerateMesh() const;

    [[nodiscard]] static int GetFlatIndex(BlocksEngine::Vector3<int> position);
    [[nodiscard]] static int GetFlatIndex(int x, int y, int z);


private:
    std::optional<ChunkData> blocks_{std::nullopt};
    const World& world_;
    const ChunkCoords coords_;

    std::vector<std::shared_ptr<ChunkSection>> sections_;
};
