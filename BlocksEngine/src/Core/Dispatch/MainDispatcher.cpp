#include "BlocksEngine/pch.h"

#include "BlocksEngine/Core/Dispatch/MainDispatchQueue.h"
#include "BlocksEngine/Exceptions/EngineException.h"

BlocksEngine::MainDispatchQueue::MainDispatchQueue()
    : mainThreadId_{std::this_thread::get_id()}
{
}

void BlocksEngine::MainDispatchQueue::HandleQueue()
{
    if (std::this_thread::get_id() != mainThreadId_)
    {
        throw ENGINE_EXCEPTION("Can only handle main queue items from the main thread");
    }

    std::unique_lock lock{lock_};

    while (!queue_.empty())
    {
        auto op = queue_.front();
        queue_.pop();

        // Unlock the mutex after we are done with messing with the queues
        lock.unlock();

        (*op)();

        lock.lock();
    }
}
