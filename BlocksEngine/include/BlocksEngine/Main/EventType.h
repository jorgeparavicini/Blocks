// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: EventType.h

#pragma once

namespace BlocksEngine
{
    enum class EventType;
}

enum class BlocksEngine::EventType : int
{
    None = 0,
    Update = 1,
    Render = 2,
    Render2D = 4
};

DEFINE_ENUM_FLAG_OPERATORS(BlocksEngine::EventType)
