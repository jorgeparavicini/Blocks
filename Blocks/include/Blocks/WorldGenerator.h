// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: WorldGenerator.h

#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <queue>

#include "ChunkAction.h"

namespace Blocks
{
    class WorldGenerator;
}

class Blocks::WorldGenerator
{
public:
    //------------------------------------------------------------------------------
    // Copy Constructor & Assignment
    //------------------------------------------------------------------------------

    WorldGenerator(const WorldGenerator&) = delete;
    WorldGenerator& operator=(const WorldGenerator&) = delete;

    //------------------------------------------------------------------------------
    // Move Constructor & Assignment
    //------------------------------------------------------------------------------

    WorldGenerator(const WorldGenerator&&) = delete;
    WorldGenerator& operator=(const WorldGenerator&&) = delete;

    //------------------------------------------------------------------------------
    // Destructors
    //------------------------------------------------------------------------------

    ~WorldGenerator();

    //------------------------------------------------------------------------------
    // Static Methods
    //------------------------------------------------------------------------------

    static void AddChunkAction(std::unique_ptr<ChunkAction> action);


private:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit WorldGenerator(size_t threadCount = 1);

    //------------------------------------------------------------------------------
    // Singleton Instance
    //------------------------------------------------------------------------------

    static WorldGenerator& Instance();

    std::mutex lock_;
    std::condition_variable workingCondition_;
    std::vector<std::thread> workerThreads_;
    bool shutdown_{false};

    std::queue<std::unique_ptr<ChunkAction>> queue_;

    void DispatchThreadHandler();
    void HandleChunkAction(std::unique_ptr<ChunkAction> action);
    void GenerateChunk(std::shared_ptr<Chunk> chunk);
};
