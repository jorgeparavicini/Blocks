#include "Blocks/pch.h"
#include "Blocks/Chunk.h"

#include "BlocksEngine/Albedo.h"
#include "BlocksEngine/Game.h"
#include "BlocksEngine/Renderer.h"
#include "BlocksEngine/Terrain.h"
#include "BlocksEngine/Vector3.h"

Blocks::Chunk::Chunk(BlocksEngine::Actor& actor)
    : Chunk{actor, BlocksEngine::Vector2{0, 0}}
{
}

Blocks::Chunk::Chunk(BlocksEngine::Actor& actor, BlocksEngine::Vector2 center)
    : Component{actor},
      center_{center}
{
}

BlocksEngine::Vector2 Blocks::Chunk::GetCenter() const
{
    return center_;
}

const Blocks::Block& Blocks::Chunk::GetBlock(const BlocksEngine::Vector3 position) const
{
    const uint8_t blockId = blocks_[GetFlatIndex(position)];
    return Block::Blocks.at(blockId);
}

int Blocks::Chunk::GetFlatIndex(BlocksEngine::Vector3 position) const
{
    static constexpr auto MaxSize = BlocksEngine::Vector3(Width, Height, Depth);
    position.Clamp(BlocksEngine::Vector3::Zero, MaxSize);

    // TODO: We need an integer Vector class.
    const int x = static_cast<int>(std::round(position.x));
    const int y = static_cast<int>(std::round(position.y));
    const int z = static_cast<int>(std::round(position.z));
    return x + Width * (y + Depth * z);
}

void Blocks::Chunk::RegenerateMesh() const
{
    // Based on the post of: https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/
    constexpr int nrBlocks = Width * Depth * Height;
    std::vector<BlocksEngine::Vertex> vertices(24);
    std::vector<int> indices(36);

    int faceNr = 0;

    for (int dim = 0; dim < 3; ++dim)
    {
        int k, w, h;
        const int u = (dim + 1) % 3;
        const int v = (dim + 2) % 3;
        int x[3] = {0, 0, 0};
        int q[3] = {0, 0, 0};

        int mask[Width * Depth];
        q[dim] = 1;

        for (x[dim] = -1; x[dim] < Width;)
        {
            int n = 0;
            for (x[v] = 0; x[v] < Width; ++x[v])
            {
                for (x[u] = 0; x[u] < Width; ++x[u], ++n)
                {
                    int a = 0 <= x[dim] ? 1 : 0;
                    int b = x[dim] < Width - 1 ? 1 : 0;

                    if (a == b)
                    {
                        mask[n] = 0;
                    }
                    else if (a)
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
            n = 0;

            for (int j = 0; j < Width; ++j)
            {
                for (int i = 0; i < Width;)
                {
                    int c = mask[n];
                    if (c)
                    {
                        for (w = 1; i + w < Width && c == mask[n + w]; w++)
                        {
                        }

                        bool done = false;
                        for (h = 1; j + h < Width; ++h)
                        {
                            for (k = 0; k < w; ++k)
                            {
                                if (c != mask[n + k + h * Width])
                                {
                                    done = true;
                                    break;
                                }
                            }

                            if (done) break;
                        }

                        x[u] = i;
                        x[v] = j;

                        int du[3] = {0, 0, 0};

                        int dv[3] = {0, 0, 0};

                        if (c > 0)
                        {
                            dv[v] = h;
                            du[u] = w;
                        }
                        else
                        {
                            c = -c;
                            du[v] = h;
                            dv[u] = w;
                        }

                        if (faceNr == 0)
                        {
                            vertices[faceNr * 4].pos = {
                                static_cast<float>(x[0]), static_cast<float>(x[1]), static_cast<float>(x[2])
                            };
                            vertices[faceNr * 4].tex = {16, 16};
                            vertices[faceNr * 4].texIndex = 0;

                            vertices[faceNr * 4 + 1].pos = {
                                static_cast<float>(x[0] + du[0]), static_cast<float>(x[1] + du[1]),
                                static_cast<float>(x[2] + du[2])
                            };
                            vertices[faceNr * 4 + 1].tex = {0, 16};
                            vertices[faceNr * 4 + 2].pos = {
                                static_cast<float>(x[0] + dv[0]), static_cast<float>(x[1] + dv[1]),
                                static_cast<float>(x[2] + dv[2])
                            };
                            vertices[faceNr * 4 + 2].tex = {16, 0};
                            vertices[faceNr * 4 + 3].pos = {
                                static_cast<float>(x[0] + du[0] + dv[0]), static_cast<float>(x[1] + du[1] + dv[1]),
                                static_cast<float>(x[2] + du[2] + dv[2])
                            };
                            vertices[faceNr * 4 + 3].tex = {0, 0};

                            indices[faceNr * 6] = faceNr * 4;
                            indices[faceNr * 6 + 1] = faceNr * 4 + 1;
                            indices[faceNr * 6 + 2] = faceNr * 4 + 2;

                            indices[faceNr * 6 + 3] = faceNr * 4 + 1;
                            indices[faceNr * 6 + 4] = faceNr * 4 + 3;
                            indices[faceNr * 6 + 5] = faceNr * 4 + 2;
                        }
                        faceNr += 1;

                        for (int l = 0; l < h; ++l)
                        {
                            for (k = 0; k < w; ++k)
                            {
                                mask[n + k + l * Width] = 0;
                            }
                        }

                        i += w;
                        n += w;
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
    /*
    for (int z = 0; z < Depth; z++)
    {
        for (int y = 0; y < Height; y++)
        {
            for (int x = 0; x < Width; x++)
            {
                const unsigned short blockNr = x + y * Width + z * Width * Height;
                const int vertexBase = blockNr * 24;
                const int indexBase = blockNr * 36;

                const Block& block = Block::Dirt;

                // Front

                vertices[vertexBase + 0].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 0].tex = {
                    block.GetTextures()[0][0] / 16.0f, (block.GetTextures()[0][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 1].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 1].tex = {
                    (block.GetTextures()[0][0] + 1.0f) / 16.0f, (block.GetTextures()[0][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 2].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 2].tex = {
                    block.GetTextures()[0][0] / 16.0f, block.GetTextures()[0][1] / 16.0f
                };

                vertices[vertexBase + 3].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 3].tex = {
                    (block.GetTextures()[0][0] + 1.0f) / 16.0f, block.GetTextures()[0][1] / 16.0f
                };

                // Right

                vertices[vertexBase + 4].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 4].tex = {
                    block.GetTextures()[1][0] / 16.0f, (block.GetTextures()[1][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 5].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 5].tex = {
                    (block.GetTextures()[1][0] + 1.0f) / 16.0f, (block.GetTextures()[1][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 6].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 6].tex = {
                    block.GetTextures()[1][0] / 16.0f, block.GetTextures()[1][1] / 16.0f
                };

                vertices[vertexBase + 7].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 7].tex = {
                    (block.GetTextures()[1][0] + 1.0f) / 16.0f, block.GetTextures()[1][1] / 16.0f
                };

                // Back

                vertices[vertexBase + 8].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 8].tex = {
                    block.GetTextures()[2][0] / 16.0f, (block.GetTextures()[2][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 9].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 9].tex = {
                    (block.GetTextures()[2][0] + 1.0f) / 16.0f, (block.GetTextures()[2][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 10].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 10].tex = {
                    block.GetTextures()[2][0] / 16.0f, block.GetTextures()[2][1] / 16.0f
                };

                vertices[vertexBase + 11].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 11].tex = {
                    (block.GetTextures()[2][0] + 1.0f) / 16.0f, block.GetTextures()[2][1] / 16.0f
                };

                // Left

                vertices[vertexBase + 12].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 12].tex = {
                    block.GetTextures()[3][0] / 16.0f, (block.GetTextures()[3][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 13].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 13].tex = {
                    (block.GetTextures()[3][0] + 1.0f) / 16.0f, (block.GetTextures()[3][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 14].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 14].tex = {
                    block.GetTextures()[3][0] / 16.0f, block.GetTextures()[3][1] / 16.0f
                };

                vertices[vertexBase + 15].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 15].tex = {
                    (block.GetTextures()[3][0] + 1.0f) / 16.0f, block.GetTextures()[3][1] / 16.0f
                };

                // Top

                vertices[vertexBase + 16].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 16].tex = {
                    block.GetTextures()[4][0] / 16.0f, (block.GetTextures()[4][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 17].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z)
                };
                vertices[vertexBase + 17].tex = {
                    (block.GetTextures()[4][0] + 1.0f) / 16.0f, (block.GetTextures()[4][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 18].pos = {
                    static_cast<float>(x), static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 18].tex = {
                    block.GetTextures()[4][0] / 16.0f, block.GetTextures()[4][1] / 16.0f
                };

                vertices[vertexBase + 19].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y) + 1.0f, static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 19].tex = {
                    (block.GetTextures()[4][0] + 1.0f) / 16.0f, block.GetTextures()[4][1] / 16.0f
                };

                // Bottom

                vertices[vertexBase + 20].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 20].tex = {
                    block.GetTextures()[5][0] / 16.0f, (block.GetTextures()[5][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 21].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z) + 1.0f
                };
                vertices[vertexBase + 21].tex = {
                    (block.GetTextures()[5][0] + 1.0f) / 16.0f, (block.GetTextures()[5][1] + 1.0f) / 16.0f
                };

                vertices[vertexBase + 22].pos = {
                    static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 22].tex = {
                    block.GetTextures()[5][0] / 16.0f, block.GetTextures()[5][1] / 16.0f
                };

                vertices[vertexBase + 23].pos = {
                    static_cast<float>(x) + 1.0f, static_cast<float>(y), static_cast<float>(z)
                };
                vertices[vertexBase + 23].tex = {
                    (block.GetTextures()[5][0] + 1.0f) / 16.0f, block.GetTextures()[5][1] / 16.0f
                };


                indices[indexBase + 0] = vertexBase + 0;
                indices[indexBase + 1] = vertexBase + 2;
                indices[indexBase + 2] = vertexBase + 1;

                indices[indexBase + 3] = vertexBase + 2;
                indices[indexBase + 4] = vertexBase + 3;
                indices[indexBase + 5] = vertexBase + 1;

                indices[indexBase + 6] = vertexBase + 4;
                indices[indexBase + 7] = vertexBase + 6;
                indices[indexBase + 8] = vertexBase + 5;

                indices[indexBase + 9] = vertexBase + 6;
                indices[indexBase + 10] = vertexBase + 7;
                indices[indexBase + 11] = vertexBase + 5;

                indices[indexBase + 12] = vertexBase + 8;
                indices[indexBase + 13] = vertexBase + 10;
                indices[indexBase + 14] = vertexBase + 9;

                indices[indexBase + 15] = vertexBase + 10;
                indices[indexBase + 16] = vertexBase + 11;
                indices[indexBase + 17] = vertexBase + 9;

                indices[indexBase + 18] = vertexBase + 12;
                indices[indexBase + 19] = vertexBase + 14;
                indices[indexBase + 20] = vertexBase + 13;

                indices[indexBase + 21] = vertexBase + 14;
                indices[indexBase + 22] = vertexBase + 15;
                indices[indexBase + 23] = vertexBase + 13;

                indices[indexBase + 24] = vertexBase + 16;
                indices[indexBase + 25] = vertexBase + 18;
                indices[indexBase + 26] = vertexBase + 17;

                indices[indexBase + 27] = vertexBase + 18;
                indices[indexBase + 28] = vertexBase + 19;
                indices[indexBase + 29] = vertexBase + 17;

                indices[indexBase + 30] = vertexBase + 20;
                indices[indexBase + 31] = vertexBase + 22;
                indices[indexBase + 32] = vertexBase + 21;

                indices[indexBase + 33] = vertexBase + 22;
                indices[indexBase + 34] = vertexBase + 23;
                indices[indexBase + 35] = vertexBase + 21;
            }
        }
    }*/

    const BlocksEngine::Graphics& gfx = GetGame().Graphics();

    auto mesh = std::make_shared<BlocksEngine::Mesh>(std::make_shared<BlocksEngine::VertexBuffer>(gfx, vertices),
                                                     std::make_shared<BlocksEngine::IndexBuffer>(gfx, indices));
    GetActor().AddComponent<BlocksEngine::Renderer>(
        std::make_shared<BlocksEngine::Terrain>(GetActor().GetGame().Graphics(),
                                                std::vector{std::wstring{L"resources/images/dirt.jpg"}}),
        std::move(mesh));
}
