#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkGroup.h"

#include "BlocksEngine/Core/Dispatch/DispatchQueue.h"

using namespace BlocksEngine;

// TODO: All pointers need to be checked for null, otherwise app go big crash

// TODO: Analyze if lock and execution has started could be converted to std::atomic
// probably not :v

void DispatchWorkGroup::AddWorkItem(std::shared_ptr<DispatchObject> workItem,
                                    std::shared_ptr<BaseDispatchQueue> queue)
{
    std::unique_lock lock{lock_};
    if (hasExecutionStarted_)
    {
        // TODO: Boost log error, work item already executing or executed.
        // And don't abort this is just for testing
        abort();
        return;
    }

    workItems_.push({std::move(queue), std::move(workItem)});
    ++nrOfWorkItems_;
}

unsigned DispatchWorkGroup::Count()
{
    std::unique_lock lock{lock_};
    return nrOfWorkItems_;
}

void DispatchWorkGroup::Execute()
{
    std::unique_lock lock{lock_};
    if (hasExecutionStarted_)
    {
        // TODO: Boost log error, work item already executing or executed.
        // And don't abort this is just for testing
        abort();
        return;
    }
    hasExecutionStarted_ = true;
    lock.unlock();

    while (!workItems_.empty())
    {
        const auto [queue, item] = workItems_.front();
        workItems_.pop();

        // Add callback to work item
        item->AddCallback(DispatchQueue::Background(),
                          std::make_shared<DispatchWorkItem>([lifetime = shared_from_this(), this]
                          {
                              std::unique_lock l{lock_};
                              ++workItemsExecuted_;

                              if (workItemsExecuted_ == nrOfWorkItems_)
                              {
                                  Notify();
                              }
                          }));

        // Add item to dispatch queue
        queue->Async(item);
    }
}


void DispatchWorkGroup::operator()()
{
    Execute();
}
