#include "Blocks/pch.h"
#include "Blocks/Chunk.h"

#include "Blocks/BlockRegistry.h"
#include "Blocks/World.h"
#include "BlocksEngine/Albedo.h"
#include "BlocksEngine/DispatchQueue.h"
#include "BlocksEngine/Game.h"
#include "BlocksEngine/Renderer.h"
#include "BlocksEngine/Terrain.h"
#include "BlocksEngine/Vector3.h"

using namespace Blocks;

Chunk::Chunk(std::weak_ptr<BlocksEngine::Actor> actor, const World& world, const ChunkCoords coords)
    : Component{std::move(actor)},
      world_{world},
      coords_{coords}
{
    GetTransform()->SetPosition({static_cast<float>(coords.x) * Width, 0, static_cast<float>(coords.y) * Depth});
}

const Block& Chunk::GetWorldBlock(const BlocksEngine::Vector3<int> position) const noexcept
{
    if (!IsInitialized())
    {
        // TODO: This needs to change
        return Block::Air;
    }
    if (position.y < 0 || position.y > Height - 1) return Block::Air;
    if (world_.ChunkCoordFromPosition(position) != coords_) return world_.GetBlock(position);

    const uint8_t blockId = blocks_.value()[GetFlatIndex(position)];
    return BlockRegistry::GetBlock(blockId);
}

const Block& Chunk::GetLocalBlock(const BlocksEngine::Vector3<int> position) const noexcept
{
    return GetWorldBlock({position.x + coords_.x * Width, position.y, position.z + coords_.y * Depth});
}

const World& Chunk::GetWorld() const noexcept
{
    return world_;
}

Chunk::ChunkCoords Chunk::GetCoords() const noexcept
{
    return coords_;
}

bool Chunk::IsInitialized() const noexcept
{
    return blocks_ != std::nullopt;
}

void Chunk::SetBlocks(std::vector<uint8_t> blocks)
{
    blocks_ = std::move(blocks);
}

inline int Chunk::GetFlatIndex(BlocksEngine::Vector3<int> position)
{
    static constexpr auto MaxSize = BlocksEngine::Vector3{Width, Height, Depth};
    // TODO: can this be optimized?
    position = position % MaxSize;
    if (position.x < 0) position.x += Width;
    if (position.y < 0) position.y += Height;
    if (position.z < 0) position.z += Depth;


    return position.x + Width * (position.y + Height * position.z);
}

int Chunk::GetFlatIndex(const int x, const int y, const int z)
{
    return GetFlatIndex({x, y, z});
}

void Chunk::RegenerateMesh() const
{
    BlocksEngine::DispatchQueue::Background().Async([this]
    {
        // Based on the post of: https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/
        // and https://github.com/Vercidium/voxel-mesh-generation

        std::vector<BlocksEngine::Vertex> vertices;
        std::vector<int> indices;

        constexpr int dimensions[3] = {Width, Height, Depth};

        // Loop over every axis (x, y, z)
        for (int dim = 0; dim < 3; ++dim)
        {
            int k;
            const int u = (dim + 1) % 3;
            const int v = (dim + 2) % 3;

            int x[3] = {0, 0, 0};
            int q[3] = {0, 0, 0};
            q[dim] = 1;

            std::vector<int> mask(dimensions[u] * dimensions[v]);

            // Check every slice of the chunk
            for (x[dim] = -1; x[dim] < dimensions[dim];)
            {
                // Compute the mask
                int n = 0;
                for (x[v] = 0; x[v] < dimensions[v]; ++x[v])
                {
                    for (x[u] = 0; x[u] < dimensions[u]; ++x[u], ++n)
                    {
                        const int a = GetLocalBlock({x[0], x[1], x[2]}).GetId();
                        const int b = GetLocalBlock({x[0] + q[0], x[1] + q[1], x[2] + q[2]}).GetId();


                        if (static_cast<bool>(a) == static_cast<bool>(b))
                        {
                            mask[n] = 0;
                        }
                        else if (static_cast<bool>(a))
                        {
                            mask[n] = a;
                        }
                        else
                        {
                            mask[n] = -b;
                        }
                    }
                }

                ++x[dim];

                // Generate mesh for the current mask
                n = 0;

                for (int j = 0; j < dimensions[v]; ++j)
                {
                    for (int i = 0; i < dimensions[u];)
                    {
                        if (int blockId = mask[n]; static_cast<bool>(blockId))
                        {
                            // Compute width of the face
                            int width = 1;
                            while (i + width < dimensions[u] && blockId == mask[static_cast<unsigned long long>(n) +
                                width])
                            {
                                ++width;
                            }

                            // Compute height of the face
                            int height = 1;
                            while (j + height < dimensions[v])
                            {
                                for (k = 0; k < width; ++k)
                                {
                                    if (blockId != mask[
                                        static_cast<unsigned long long>(n)
                                        + k
                                        + static_cast<unsigned long long>(height)
                                        * dimensions[u]])
                                    {
                                        goto afterLoop;
                                    }
                                }
                                ++height;
                            }
                        afterLoop:

                            x[u] = i;
                            x[v] = j;

                            // Determine the size and orientation of this face
                            int du[3] = {0, 0, 0};
                            int dv[3] = {0, 0, 0};

                            const bool flipUvs = q[0] == 1 && blockId > 0
                                || q[1] == 1 && blockId < 0
                                || q[2] == 1 && blockId < 0;

                            const uint8_t faceId = q[0] * (blockId < 0 ? 3 : 1)
                                + q[1] * (blockId < 0 ? 5 : 4)
                                + q[2] * (blockId < 0 ? 2 : 0);


                            // If blockId is less than 0 the face is flipped
                            if (blockId > 0)
                            {
                                dv[v] = height;
                                du[u] = width;
                            }
                            else
                            {
                                blockId = -blockId;
                                du[v] = height;
                                dv[u] = width;
                            }

                            int uv[2] = {0, 0};

                            if (q[0] == 1)
                            {
                                uv[0] = width;
                                uv[1] = height;
                            }
                            else
                            {
                                uv[0] = height;
                                uv[1] = width;
                            }

                            const Block& block = BlockRegistry::GetBlock(blockId);

                            int vertexCount = static_cast<int>(vertices.size());
                            vertices.push_back(BlocksEngine::Vertex{
                                {
                                    static_cast<float>(x[0]),
                                    static_cast<float>(x[1]),
                                    static_cast<float>(x[2])
                                },
                                {static_cast<float>(flipUvs ? 0 : uv[1]), static_cast<float>(uv[0])},
                                block.GetTextures()[faceId]
                            });

                            vertices.push_back(BlocksEngine::Vertex{
                                {
                                    static_cast<float>(x[0] + du[0]),
                                    static_cast<float>(x[1] + du[1]),
                                    static_cast<float>(x[2] + du[2])
                                },
                                {static_cast<float>(0), static_cast<float>(flipUvs ? 0 : uv[0])},
                                block.GetTextures()[faceId]
                            });

                            vertices.push_back(BlocksEngine::Vertex{
                                {
                                    static_cast<float>(x[0] + dv[0]),
                                    static_cast<float>(x[1] + dv[1]),
                                    static_cast<float>(x[2] + dv[2])
                                },
                                {static_cast<float>(uv[1]), static_cast<float>(flipUvs ? uv[0] : 0)},
                                block.GetTextures()[faceId]
                            });

                            vertices.push_back(BlocksEngine::Vertex{
                                {
                                    static_cast<float>(x[0] + du[0] + dv[0]),
                                    static_cast<float>(x[1] + du[1] + dv[1]),
                                    static_cast<float>(x[2] + du[2] + dv[2])
                                },
                                {static_cast<float>(flipUvs ? uv[1] : 0), static_cast<float>(0)},
                                block.GetTextures()[faceId]
                            });

                            indices.push_back(vertexCount);
                            indices.push_back(vertexCount + 1);
                            indices.push_back(vertexCount + 2);

                            indices.push_back(vertexCount + 1);
                            indices.push_back(vertexCount + 3);
                            indices.push_back(vertexCount + 2);


                            for (int l = 0; l < height; ++l)
                            {
                                for (k = 0; k < width; ++k)
                                {
                                    mask[static_cast<unsigned long long>(n) + k + static_cast<unsigned long long>(l) *
                                        dimensions[u]] = 0;
                                }
                            }

                            i += width;
                            n += width;
                        }
                        else
                        {
                            i++;
                            n++;
                        }
                    }
                }
            }
        }


        const BlocksEngine::Graphics& gfx = GetGame()->Graphics();

        auto mesh = std::make_shared<BlocksEngine::Mesh>(std::make_shared<BlocksEngine::VertexBuffer>(gfx, vertices),
                                                         std::make_shared<BlocksEngine::IndexBuffer>(gfx, indices));
        GetGame()->MainDispatchQueue().Async([this, mesh]
        {
            GetActor()->AddComponent<BlocksEngine::Renderer>(
                std::make_shared<BlocksEngine::Terrain>(GetActor()->GetGame()->Graphics(),
                                                        BlocksEngine::Texture2D::FromDds(
                                                            GetGame()->Graphics(), L"resources/images/terrain.dds")),
                mesh);
        });
    });
}

std::ostream& Blocks::operator<<(std::ostream& out, const Chunk& chunk)
{
    out << chunk.coords_.x << ", " << chunk.coords_.y;
    return out;
}
