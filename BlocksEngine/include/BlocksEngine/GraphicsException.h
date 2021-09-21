// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GraphicsException.h

#pragma once
#include "Exception.h"

namespace BlocksEngine
{
    class GraphicsException;
}

class BlocksEngine::GraphicsException final : public Exception
{
public:
    [[nodiscard]] const char* GetType() const noexcept override;
};
