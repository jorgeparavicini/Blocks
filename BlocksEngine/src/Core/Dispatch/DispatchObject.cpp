#include "BlocksEngine/pch.h"
#include "BlocksEngine/DispatchObject.h"

#include "BlocksEngine/BaseDispatchQueue.h"

using namespace BlocksEngine;

void DispatchObject::AddCallback(std::shared_ptr<BaseDispatchQueue> queue,
                                 std::shared_ptr<DispatchWorkItem> workItem)
{
    std::unique_lock lock{lock_};
    if (hasExecutionStarted_)
    {
        // TODO: Boost log error, work item already executing or executed.
        // And don't abort this is just for testing
        abort();
        return;
    }

    callbacks_.push({std::move(queue), std::move(workItem)});
}

void DispatchObject::AddCallback(std::shared_ptr<DispatchWorkItem> workItem)
{
    AddCallback(nullptr, std::move(workItem));
}

void DispatchObject::Notify()
{
    while (!callbacks_.empty())
    {
        // TODO: move?
        auto [queue, callback] = callbacks_.front();
        callbacks_.pop();

        if (queue)
        {
            queue->Async(std::move(callback));
        }
        else
        {
            (*callback)();
        }
    }
}
