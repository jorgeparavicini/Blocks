// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DispatchQueue.h

#pragma once

#include <condition_variable>
#include <functional>
#include <queue>

#include "BaseDispatchQueue.h"

namespace BlocksEngine
{
    class DispatchQueue;
}

class BlocksEngine::DispatchQueue final : BaseDispatchQueue
{
public:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit DispatchQueue(size_t threadCount = 1);

    //------------------------------------------------------------------------------
    // Copy Constructor & Assignment
    //------------------------------------------------------------------------------

    DispatchQueue(const DispatchQueue&) = delete;
    DispatchQueue& operator=(const DispatchQueue&) = delete;

    //------------------------------------------------------------------------------
    // Move Constructor & Assignment
    //------------------------------------------------------------------------------

    DispatchQueue(const DispatchQueue&&) = delete;
    DispatchQueue& operator=(const DispatchQueue&&) = delete;

    //------------------------------------------------------------------------------
    // Destructors
    //------------------------------------------------------------------------------

    ~DispatchQueue() override;

    //------------------------------------------------------------------------------
    // Global Queues
    //------------------------------------------------------------------------------

    static DispatchQueue& Background();

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void Async(std::function<void()> workItem) override;


private:
    bool shutdown_{false};

    std::condition_variable workingCondition_;
    std::vector<std::thread> workerThreads_;

    void DispatchThreadHandler();
};
