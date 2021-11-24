// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DispatchObject.h

#pragma once
#include <mutex>
#include <queue>


namespace BlocksEngine
{
    class BaseDispatchQueue;
    class DispatchWorkItem;

    class DispatchObject;
}

class BlocksEngine::DispatchObject : public std::enable_shared_from_this<DispatchObject>
{
public:
    /**
    * \brief Registers a callback on a given queue that gets called once all work items in this group have been executed.
    *  
    * \param queue The dispatch queue the callback should be executed on.
    * \param workItem The operation to be executed.
    */
    void AddCallback(std::shared_ptr<BaseDispatchQueue> queue, std::shared_ptr<DispatchWorkItem> workItem);

    /**
    * \brief Registers a callback synchronously that gets called once all work items in this group have been executed.
    * \remark The callback will be called on the thread of the last work item that got executed.
    *
    * \param workItem The operation to be executed.
    */
    void AddCallback(std::shared_ptr<DispatchWorkItem> workItem);

protected:
    std::mutex lock_;
    bool hasExecutionStarted_{false};

    std::queue<std::pair<std::shared_ptr<BaseDispatchQueue>, std::shared_ptr<DispatchWorkItem>>> callbacks_{};

    void Notify();
};
