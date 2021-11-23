#include "BlocksEngine/pch.h"
#include "BlocksEngine/DispatchWorkItem.h"

BlocksEngine::DispatchWorkItem::DispatchWorkItem(WorkOperation operation)
    : operation_{operation}
{
}

void BlocksEngine::DispatchWorkItem::Execute()
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

    operation_();

    Notify();
}

void BlocksEngine::DispatchWorkItem::operator()()
{
    Execute();
}
