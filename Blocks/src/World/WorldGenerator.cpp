#include "Blocks/pch.h"
#include "Blocks/WorldGenerator.h"

Blocks::WorldGenerator::WorldGenerator(const size_t threadCount)
    : workerThreads_(threadCount)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workerThreads_[i] = std::thread(&DispatchThreadHandler, this);
    }
}

Blocks::WorldGenerator::~WorldGenerator()
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

void Blocks::WorldGenerator::AddChunkAction(std::unique_ptr<ChunkAction> action)
{
    std::unique_lock lock{Instance().lock_};
    Instance().queue_.push(std::move(action));

    lock.unlock();
    Instance().workingCondition_.notify_one();
}

inline Blocks::WorldGenerator& Blocks::WorldGenerator::Instance()
{
    [[clang::always_destroy]]
        static WorldGenerator instance; // NOLINT(clang-diagnostic-exit-time-destructors)

    return instance;
}

void Blocks::WorldGenerator::DispatchThreadHandler()
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
            std::unique_ptr<ChunkAction> op = std::move(queue_.front());
            queue_.pop();

            // Unlock the mutex after we are done with messing with the queues
            lock.unlock();

            HandleChunkAction(std::move(op));

            lock.lock();
        }
    }
    while (!shutdown_);
}

void Blocks::WorldGenerator::HandleChunkAction(const std::unique_ptr<ChunkAction> action)
{
    switch (action->type)
    {
    case ChunkAction::Type::Generate:
        GenerateChunk(std::move(action->chunk));

    case ChunkAction::Type::Mesh:
        break; // TODO: Implement
    }
}

void Blocks::WorldGenerator::GenerateChunk(std::shared_ptr<Chunk> chunk)
{
    std::array<
    for (int i = 0; i < Chunk::Width; i++)
    {
        for (int j = 0; j < Chunk::Height; j++)
        {
            for (int k = 0; k < Chunk::Depth; k++)
            {
                chunk->blocks_[chunk->GetFlatIndex(i, j, k)] = j == Chunk::Height - 1 ? 2 : 1;

                if (j == Chunk::Height - 1 && i % 2 == 0 && k % 2 == 0)
                {
                    blocks_[GetFlatIndex(i, j, k)] = 0;
                }
            }
        }
    }
}
