#include "Blocks/pch.h"
#include "Blocks/World.h"

#include <boost/log/trivial.hpp>

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/DispatchQueue.h"
#include "BlocksEngine/Game.h"

using namespace Blocks;
using namespace BlocksEngine;

World::World(std::weak_ptr<Actor> actor, std::weak_ptr<Transform> playerTransform,
             const uint8_t chunkLoadDistance)
    : Component{std::move(actor)},
      chunkViewDistance_{chunkLoadDistance},
      playerTransform_{std::move(playerTransform)}
{
    GenerateWorld();
}

void World::Update()
{
    const auto transform = playerTransform_.lock();
    if (!transform) return;
    const Chunk::ChunkCoords chunkCoords = ChunkCoordFromPosition(transform->GetPosition());

    if (chunkCoords != lastChunkCoords_)
    {
        //UpdateChunks();
    }
}

// TODO: Test if this could be moved
void World::SetPlayerTransform(const std::shared_ptr<Transform> playerTransform) noexcept
{
    playerTransform_ = playerTransform;
}

Vector2<int> World::ChunkCoordFromPosition(
    const Vector3<float>& position) const noexcept
{
    int x = static_cast<int>(std::floor(position.x / Chunk::Width));
    int z = static_cast<int>(std::floor(position.z / Chunk::Depth));
    return {x, z};
}

const Block& World::GetBlock(const Vector3<int> position) const noexcept
{
    const Chunk::ChunkCoords coords = ChunkCoordFromPosition(position);
    const auto chunk = chunkMap_.find(coords);
    if (chunk == chunkMap_.end())
    {
        return Block::Air;
    }

    return chunk->second->GetWorldBlock(position);
}

void World::GenerateWorld() noexcept
{
    BOOST_LOG_TRIVIAL(debug) << "Starting World Generator";
    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            const auto chunk = CreateChunk({i - chunkViewDistance_ / 2, j - chunkViewDistance_ / 2});
            GenerateChunk(chunk);
        }
    }
}

std::shared_ptr<Chunk> World::CreateChunk(Chunk::ChunkCoords coords)
{
    const std::shared_ptr<Actor> actor = GetGame()->AddActor(
        L"Chunk: " + std::to_wstring(coords.x) + L", " + std::to_wstring(coords.y));
    const std::shared_ptr<Chunk> chunk = actor->AddComponent<Chunk>(*this, coords);
    chunkMap_[coords] = chunk;
    return chunk;
}

void World::GenerateChunk(std::shared_ptr<Chunk> chunk)
{
    std::unique_lock lock{genLock_};

    if (generatingChunks_.contains(chunk->GetCoords()))
    {
        BOOST_LOG_TRIVIAL(error)
            << "Chunk generation request received for a chunk that is already generating. Chunk "
            << *chunk.get();
        // Returning unlocks the lock again
        return;
    }

    generatingChunks_.insert(chunk->GetCoords());
    lock.unlock();

    BOOST_LOG_TRIVIAL(trace) << "Queuing chunk generation for chunk at " << *chunk.get();
    DispatchQueue::Background().Async([this, chunk, &lock]
    {
        BOOST_LOG_TRIVIAL(debug) << "Generating chunk at " << *chunk.get();
        auto blocks{
            std::make_shared<std::vector<uint8_t>>(Chunk::Size)
        };
        for (int i = 0; i < Chunk::Width; i++)
        {
            for (int j = 0; j < Chunk::Height; j++)
            {
                for (int k = 0; k < Chunk::Depth; k++)
                {
                    (*blocks)[Chunk::GetFlatIndex(i, j, k)] = j == Chunk::Height - 1 ? 2 : 1;

                    if (j == Chunk::Height - 1 && i % 2 == 0 && k % 2 == 0)
                    {
                        (*blocks)[Chunk::GetFlatIndex(i, j, k)] = 0;
                    }
                }
            }
        }

        GetGame()->MainDispatchQueue().Async([this, chunk, blocks, &lock]
        {
            chunk->SetBlocks(std::move(*blocks));
            lock.lock();
            generatingChunks_.erase(chunk->GetCoords());
            if (generatingChunks_.empty())
            {
                for (auto chunkMap : chunkMap_)
                {
                    chunkMap.second->RegenerateMesh();
                }
            }
            lock.unlock();
        });
    });
}

void World::UpdateChunks()
{
    const Chunk::ChunkCoords playerCoords = ChunkCoordFromPosition(playerTransform_.lock()->GetPosition());
    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            const Chunk::ChunkCoords chunkCoords = Vector2(
                i - chunkViewDistance_ / 2, j - chunkViewDistance_ / 2) + playerCoords;
            const auto chunk = chunkMap_.find(chunkCoords);
            if (chunk == chunkMap_.end())
            {
                const auto c = CreateChunk(chunkCoords);
                c->RegenerateMesh();
            }
        }
    }
}
