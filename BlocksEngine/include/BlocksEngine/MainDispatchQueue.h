// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: MainDispatchQueue.h

#pragma once

#include "BlocksEngine/BaseDispatchQueue.h"

namespace BlocksEngine
{
    class MainDispatchQueue;
}

class BlocksEngine::MainDispatchQueue final : public BaseDispatchQueue
{
public:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    MainDispatchQueue();

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void HandleQueue();

private:
    std::thread::id mainThreadId_;
};
