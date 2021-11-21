#include "BlocksEngine/pch.h"
#include "BlocksEngine/DispatchQueue.h"

using namespace BlocksEngine;

DispatchQueue::DispatchQueue(const size_t threadCount)
    : BaseDispatchQueue{},
      workerThreads_(threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workerThreads_[i] = std::thread(&DispatchQueue::DispatchThreadHandler, this);
    }
}

DispatchQueue::~DispatchQueue()
{
    // Signal all worker threads to clean up their work.
    shutdown_ = true;
    workingCondition_.notify_all();

    // Shutdown the threads
    for (auto& workerThread : workerThreads_)
    {
        if (workerThread.joinable())
        {
            workerThread.join();
        }
    }
}

DispatchQueue& DispatchQueue::Background()
{
    static DispatchQueue instance{4}; // NOLINT(clang-diagnostic-exit-time-destructors)

    return instance;
}

void DispatchQueue::Async(std::function<void()> workItem)
{
    BaseDispatchQueue::Async(std::move(workItem));
    workingCondition_.notify_one();
}

void DispatchQueue::DispatchThreadHandler()
{
    std::unique_lock lock{lock_};

    do
    {
        // Wait until new operations have been submitted
        workingCondition_.wait(lock, [this]
        {
            return !queue_.empty() || shutdown_;
        });

        // We own the lock after the wait
        if (!queue_.empty())
        {
            std::function<void()> op = std::move(queue_.front());
            queue_.pop();

            // Unlock the mutex after we are done with messing with the queues
            lock.unlock();

            op();

            lock.lock();
        }
    }
    while (!shutdown_);
}
