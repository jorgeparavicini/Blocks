// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DispatchWorkItem.h

#pragma once

#include <functional>

#include "BlocksEngine/Core/Dispatch/DispatchObject.h"

// TODO: ALlow returning values

namespace BlocksEngine
{
    class BaseDispatchQueue;

    class DispatchWorkItem;
}

class BlocksEngine::DispatchWorkItem : public DispatchObject
{
public:
    using WorkOperation = std::function<void()>;

    DispatchWorkItem(WorkOperation operation);

    /**
    * \brief Executes the operation of this work item synchronously on the current thread.
    */
    void Execute();

    /**
    * \brief Executes the operation of this work item synchronously on the current thread.
    */
    void operator()();

private:
    WorkOperation operation_;
};
