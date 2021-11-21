// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: BaseDispatchQueue.h

#pragma once

#include <functional>
#include <mutex>
#include <queue>

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

    virtual ~BaseDispatchQueue();

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    virtual void Async(std::function<void()> workItem);

protected:
    std::mutex lock_;
    std::queue<std::function<void()>> queue_{};
};
