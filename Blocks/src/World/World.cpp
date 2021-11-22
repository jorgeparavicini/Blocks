#include "Blocks/pch.h"
#include "Blocks/World.h"

#include <shared_mutex>
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

    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();
    HRESULT hr;
    GFX_THROW_INFO(renderTarget.CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &pBrush_));
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

void World::Draw2D()
{
    auto size = GetGame()->Graphics().Size();
    ID2D1RenderTarget& renderTarget = GetGame()->Graphics().Get2DRenderTarget();
    const D2D1_RECT_F rect = D2D1::RectF(0.0f, 0.0f, size.x - 1, size.y - 1);
    renderTarget.FillRectangle(&rect, pBrush_.Get());
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
            InitializeChunk(chunk);
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

void World::InitializeChunk(std::shared_ptr<Chunk> chunk)
{
    std::unique_lock lock{genLock_};

    if (generatingChunks_.contains(chunk->GetCoords()))
    {
        BOOST_LOG_TRIVIAL(error)
            << "Chunk initialization request received for a chunk that is already generating. Chunk "
            << *chunk.get();
        // Returning unlocks the lock again
        return;
    }

    generatingChunks_.insert(chunk->GetCoords());
    lock.unlock();

    BOOST_LOG_TRIVIAL(trace) << "Queuing chunk generation for chunk at " << *chunk.get();
    DispatchQueue::Background().Async([this, chunk]
    {
        BOOST_LOG_TRIVIAL(debug) << "Generating chunk at " << *chunk.get();

        auto blocks = GenerateChunk(chunk);

        GetGame()->MainDispatchQueue().Async([this, chunk, blocks = move(blocks)]()
        mutable
            {
                chunk->SetBlocks(std::move(blocks));
                OnChunkGenerated(chunk);
            });
    });
}


void World::OnChunkGenerated(const std::shared_ptr<Chunk> chunk)
{
    std::unique_lock dispatchLock{genLock_};

    generatingChunks_.erase(chunk->GetCoords());
    if (generatingChunks_.empty())
    {
        OnWorldGenerated();
    }
}

void World::OnWorldGenerated()
{
    for (const auto& [_, snd] : chunkMap_)
    {
        std::unique_lock lock{meshLock_};
        meshingChunks_.insert(snd->GetCoords());
        lock.unlock();
        // TODO: Extrapolate into function "RequestMeshingForChunk"
        snd->RegenerateMesh([this, &snd]
        {
            std::unique_lock l{meshLock_};
            meshingChunks_.erase(snd->GetCoords());
            if (meshingChunks_.empty())
            {
                OnWorldLoaded();
            }
        });
    }
}

void World::OnWorldLoaded()
{
    BOOST_LOG_TRIVIAL(info) << "World successfully loaded";
}

std::vector<uint8_t> World::GenerateChunk(std::shared_ptr<Chunk> chunk) const
{
    std::vector<uint8_t> blocks(Chunk::Size);
    for (int i = 0; i < Chunk::Width; i++)
    {
        for (int j = 0; j < Chunk::Height; j++)
        {
            for (int k = 0; k < Chunk::Depth; k++)
            {
                blocks[Chunk::GetFlatIndex(i, j, k)] = j == Chunk::Height - 1 ? 2 : 1;

                if (j == Chunk::Height - 1 && i % 2 == 0 && k % 2 == 0)
                {
                    blocks[Chunk::GetFlatIndex(i, j, k)] = 0;
                }
            }
        }
    }
    return blocks;
}

void World::UpdateChunks()
{
    // TODO: This needs to be reowrked
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
                //c->RegenerateMesh();
            }
        }
    }
}
