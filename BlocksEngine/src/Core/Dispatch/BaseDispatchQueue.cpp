#include "BlocksEngine/pch.h"
#include "BlocksEngine/BaseDispatchQueue.h"


BlocksEngine::BaseDispatchQueue::~BaseDispatchQueue()
{
}

void BlocksEngine::BaseDispatchQueue::Async(std::function<void()> workItem)
{
    std::unique_lock lock{lock_};
    queue_.push(std::move(workItem));

    lock.unlock();
}
