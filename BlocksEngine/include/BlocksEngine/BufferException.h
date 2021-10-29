// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: BufferException.h

#pragma once
#include "BlocksEngine/EngineException.h"

#define BUFFER_EXCEPTION(msg) BufferException(__LINE__, __FILE__, msg)

namespace BlocksEngine
{
    class BufferException;
}

class BlocksEngine::BufferException final : public EngineException
{
public:
    BufferException(int line, std::string file, std::string msg) noexcept;
    [[nodiscard]] const char* GetType() const noexcept override;
};
