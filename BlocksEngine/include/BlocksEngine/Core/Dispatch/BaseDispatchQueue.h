// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: BaseDispatchQueue.h

#pragma once

#include <mutex>
#include <queue>

#include "BlocksEngine/Core/Dispatch/DispatchWorkItem.h"

namespace BlocksEngine
{
    class BaseDispatchQueue;
}

class BlocksEngine::BaseDispatchQueue
{
public:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    BaseDispatchQueue() = default;

    //------------------------------------------------------------------------------
    // Copy Constructor & Assignment
    //------------------------------------------------------------------------------

    BaseDispatchQueue(const BaseDispatchQueue&) = delete;
    BaseDispatchQueue& operator=(const BaseDispatchQueue&) = delete;

    //------------------------------------------------------------------------------
    // Move Constructor & Assignment
    //------------------------------------------------------------------------------

    BaseDispatchQueue(const BaseDispatchQueue&&) = delete;
    BaseDispatchQueue& operator=(const BaseDispatchQueue&&) = delete;

    //------------------------------------------------------------------------------
    // Destructors
    //------------------------------------------------------------------------------

    virtual ~BaseDispatchQueue() = default;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    virtual void Async(std::shared_ptr<DispatchWorkItem> workItem);

protected:
    std::mutex lock_;
    std::queue<std::shared_ptr<DispatchWorkItem>> queue_{};
};
