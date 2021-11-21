// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ChunkAction.h

#pragma once
#include "Chunk.h"

namespace Blocks
{
    struct ChunkAction;
}

struct Blocks::ChunkAction
{
    enum class Type
    {
        Generate,
        Mesh
    };

    ChunkAction(const Type type, std::shared_ptr<Chunk> chunk)
        : type{type}, chunk{std::move(chunk)}
    {
    }

    Type type;
    std::shared_ptr<Chunk> chunk;
};
