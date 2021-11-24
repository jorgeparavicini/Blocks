// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DispatchWorkGroup.h

#pragma once
#include <mutex>
#include <queue>

#include "BlocksEngine/Core/Dispatch/DispatchObject.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkItem.h"

namespace BlocksEngine
{
    class BaseDispatchQueue;

    class DispatchWorkGroup;
}

class BlocksEngine::DispatchWorkGroup : public DispatchObject
{
public:
    void AddWorkItem(std::shared_ptr<DispatchWorkItem> workItem, std::shared_ptr<BaseDispatchQueue> queue);

    [[nodiscard]] unsigned int Count();

    void Execute();

    void operator()();
private:
    int nrOfWorkItems_{0};
    int workItemsExecuted_{0};

    std::queue<std::pair<std::shared_ptr<BaseDispatchQueue>, std::shared_ptr<DispatchWorkItem>>> workItems_{};
};
