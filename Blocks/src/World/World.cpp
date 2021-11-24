#include "Blocks/pch.h"
#include "Blocks/World/World.h"

#include <shared_mutex>
#include <boost/log/trivial.hpp>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Dispatch/DispatchQueue.h"
#include "BlocksEngine/Core/Dispatch/DispatchWorkGroup.h"
#include "BlocksEngine/Main/Game.h"

using namespace Blocks;
using namespace BlocksEngine;

World::World(std::weak_ptr<Actor> actor, std::weak_ptr<Transform> playerTransform,
             const uint8_t chunkLoadDistance)
    : Component{std::move(actor)},
      chunkViewDistance_{chunkLoadDistance},
      playerTransform_{std::move(playerTransform)},
      loadingScreen_{GetActor()->AddComponent<LoadingScreen>()}
{
    GenerateWorld();
    loadingScreen_->LevelLoaded();
}

void World::Update()
{
    const auto transform = playerTransform_.lock();
    if (!transform) return;
    const Chunk::ChunkCoords chunkCoords = ChunkCoordFromPosition(transform->GetPosition());

    if (chunkCoords != lastChunkCoords_)
    {
        UpdateChunks();
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
    const auto chunk = chunks_.find(coords);
    if (chunk == chunks_.end())
    {
        return Block::Air;
    }

    return chunk->second->GetWorldBlock(position);
}

void World::GenerateWorld() noexcept
{
    BOOST_LOG_TRIVIAL(debug) << "Starting World Generator";
    const auto workGroup{std::make_shared<DispatchWorkGroup>()};

    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            const auto chunk = CreateChunk({i - chunkViewDistance_ / 2, j - chunkViewDistance_ / 2});
            activeChunkCoords_.insert(chunk->GetCoords());
            workGroup->AddWorkItem(CreateGenerationRequestForChunk(chunk), DispatchQueue::Background());
        }
    }

    workGroup->AddCallback(GetGame()->MainDispatchQueue(), std::make_shared<DispatchWorkItem>([this]
    {
        OnWorldGenerated();
    }));

    workGroup->Execute();
}

void World::OnWorldGenerated()
{
    BOOST_LOG_TRIVIAL(info) << "World generated";

    std::vector<std::shared_ptr<Chunk>> chunks(chunks_.size());
    std::ranges::transform(chunks_, chunks.begin(), [](auto pair) { return pair.second; });
    const auto meshRequestGroup = CreateMeshRequestGroup(std::move(chunks));

    meshRequestGroup->AddCallback(GetGame()->MainDispatchQueue(), std::make_shared<DispatchWorkItem>([this]
    {
        OnWorldLoaded();
    }));
    meshRequestGroup->Execute();
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

std::shared_ptr<Chunk> World::CreateChunk(Chunk::ChunkCoords coords)
{
    const std::shared_ptr<Actor> actor = GetGame()->AddActor(
        L"Chunk: " + std::to_wstring(coords.x) + L", " + std::to_wstring(coords.y));
    const std::shared_ptr<Chunk> chunk = actor->AddComponent<Chunk>(*this, coords);
    chunks_[coords] = chunk;

    return chunk;
}

// TODO: Notify will get called before the blocks are assigned which is wrong
std::shared_ptr<DispatchWorkItem> World::CreateGenerationRequestForChunk(
    std::shared_ptr<Chunk> chunk) const
{
    return std::make_shared<DispatchWorkItem>([this, chunk]
    {
        auto blocks = GenerateChunk(chunk);

        BOOST_LOG_TRIVIAL(debug) << "Generating Chunk: " << chunk;

        const auto workItem = std::make_shared<DispatchWorkItem>([chunk, blocks = move(blocks)]()
        mutable
            {
                chunk->SetBlocks(std::move(blocks));
                BOOST_LOG_TRIVIAL(debug) << "Blocks assigned for chunk: " << chunk;
            });
        GetGame()->MainDispatchQueue()->Async(workItem);
    });
}

std::shared_ptr<DispatchWorkGroup> World::CreateMeshRequestGroup(
    const std::vector<std::shared_ptr<Chunk>> chunks) const
{
    auto workGroup = std::make_shared<DispatchWorkGroup>();

    for (auto& chunk : chunks)
    {
        workGroup->AddWorkItem(chunk->RegenerateMesh(), DispatchQueue::Background());
    }

    return workGroup;
}

void World::OnWorldLoaded() const
{
    BOOST_LOG_TRIVIAL(info) << "World successfully loaded";
    loadingScreen_->LevelLoaded();
}

void World::UpdateChunks()
{
    const auto workGroup = std::make_shared<DispatchWorkGroup>();
    const Chunk::ChunkCoords playerCoords = ChunkCoordFromPosition(playerTransform_.lock()->GetPosition());
    std::vector<std::shared_ptr<Chunk>> chunks{};

    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            const Chunk::ChunkCoords chunkCoords = Vector2(
                i - chunkViewDistance_ / 2, j - chunkViewDistance_ / 2) + playerCoords;
            const auto chunk = chunks_.find(chunkCoords);
            if (chunk == chunks_.end())
            {
                const auto c = CreateChunk(chunkCoords);
                chunks.push_back(c);
                workGroup->AddWorkItem(CreateGenerationRequestForChunk(c), DispatchQueue::Background());
            }
        }
    }

    workGroup->AddCallback(GetGame()->MainDispatchQueue(),
                           std::make_shared<DispatchWorkItem>([this, chunks = move(chunks)]()
                           mutable
                               {
                                   const auto meshRequestGroup = CreateMeshRequestGroup(std::move(chunks));
                                   meshRequestGroup->Execute();
                               }));

    workGroup->Execute();
}
