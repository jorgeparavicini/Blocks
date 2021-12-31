// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: EventType.h

#pragma once
#include "EventType.h"

namespace BlocksEngine
{
    enum class EventType;
}

enum class BlocksEngine::EventType
{
    None = 0,
    Update = 1,
    Render = 2,
    Render2D = 4,
    PhysicsUpdated = 8
};

inline BlocksEngine::EventType operator|(BlocksEngine::EventType a, BlocksEngine::EventType b)
{
    return static_cast<BlocksEngine::EventType>(static_cast<int>(a) | static_cast<int>(b));
}

inline BlocksEngine::EventType operator&(BlocksEngine::EventType a, BlocksEngine::EventType b)
{
    return static_cast<BlocksEngine::EventType>(static_cast<int>(a) & static_cast<int>(b));
}

inline BlocksEngine::EventType& operator|=(BlocksEngine::EventType& a, const BlocksEngine::EventType b)
{
    return a = a | b;
}
