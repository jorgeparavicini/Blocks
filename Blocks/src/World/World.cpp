#include "Blocks/pch.h"
#include "Blocks/World.h"

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Game.h"

Blocks::World::World(std::weak_ptr<BlocksEngine::Actor> actor, std::weak_ptr<BlocksEngine::Transform> playerTransform,
                     const uint8_t chunkLoadDistance)
    : Component{std::move(actor)},
      chunkViewDistance_{chunkLoadDistance},
      playerTransform_{std::move(playerTransform)}
{
    GenerateWorld();
}

void Blocks::World::Update()
{
    const auto transform = playerTransform_.lock();
    if (!transform) return;
    const Chunk::ChunkCoords chunkCoords = ChunkCoordFromPosition(transform->GetPosition());

    if (chunkCoords != lastChunkCoords_)
    {
        UpdateChunks();
    }
}

/*void Blocks::World::SetPlayerTransform(std::shared_ptr<BlocksEngine::Transform> playerTransform) noexcept
{
    playerTransform_ = playerTransform;
}*/

BlocksEngine::Vector2<int> Blocks::World::ChunkCoordFromPosition(
    const BlocksEngine::Vector3<float>& position) const noexcept
{
    int x = static_cast<int>(std::floor(position.x / Chunk::Width));
    int z = static_cast<int>(std::floor(position.z / Chunk::Depth));
    return {x, z};
}

const Blocks::Block& Blocks::World::GetBlock(const BlocksEngine::Vector3<int> position) const noexcept
{
    const Chunk::ChunkCoords coords = ChunkCoordFromPosition(position);
    const auto chunk = chunkMap_.find(coords);
    if (chunk == chunkMap_.end())
    {
        return Block::Air;
    }

    return chunk->second->GetWorldBlock(position);
}

void Blocks::World::GenerateWorld() noexcept
{
    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            CreateChunk({i - chunkViewDistance_ / 2, j - chunkViewDistance_ / 2});
        }
    }
    for (const auto& [fst, snd] : chunkMap_)
    {
        snd->RegenerateMesh();
    }
}

std::shared_ptr<Blocks::Chunk> Blocks::World::CreateChunk(Chunk::ChunkCoords coords)
{
    const std::shared_ptr<BlocksEngine::Actor> actor = GetGame()->AddActor(
        L"Chunk: " + std::to_wstring(coords.x) + L", " + std::to_wstring(coords.y));
    const std::shared_ptr<Chunk> chunk = actor->AddComponent<Chunk>(*this, coords);
    // TODO: WTF IS THIS i should never have to call this manually.
    //chunk->RegenerateMesh();
    chunkMap_[coords] = chunk;
    return chunk;
}

void Blocks::World::UpdateChunks()
{
    const Chunk::ChunkCoords playerCoords = ChunkCoordFromPosition(playerTransform_.lock()->GetPosition());
    for (int i = 0; i < chunkViewDistance_; i++)
    {
        for (int j = 0; j < chunkViewDistance_; j++)
        {
            const Chunk::ChunkCoords chunkCoords = BlocksEngine::Vector2(
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
