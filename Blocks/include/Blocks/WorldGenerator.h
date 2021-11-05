// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: WorldGenerator.h

#pragma once
#include <cstdint>
#include <unordered_map>

#include "BlocksEngine/Component.h"

namespace Blocks
{
    class WorldGenerator;
}

class Blocks::WorldGenerator : BlocksEngine::Component
{
public:
private:
    uint8_t chunkLoadDistance_{8};
    //std::unordered_map<
};
