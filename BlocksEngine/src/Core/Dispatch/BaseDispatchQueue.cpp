#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Dispatch/BaseDispatchQueue.h"

using namespace BlocksEngine;


void BaseDispatchQueue::Async(std::shared_ptr<DispatchWorkItem> workItem)
{
    std::unique_lock lock{lock_};
    queue_.push(std::move(workItem));

    lock.unlock();
}
