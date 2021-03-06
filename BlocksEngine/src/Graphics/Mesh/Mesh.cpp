#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/Mesh.h"

#include "BlocksEngine/Core/Math/Vertex.h"


BlocksEngine::Mesh::Mesh(const Graphics& gfx)
{
    constexpr float side = 1.0f / 2.0f;

    std::vector<Vertex> vertices(8);
    vertices[0].pos = {-side, -side, -side};
    vertices[1].pos = {side, -side, -side};
    vertices[2].pos = {-side, side, -side};
    vertices[3].pos = {side, side, -side};
    vertices[4].pos = {-side, -side, side};
    vertices[5].pos = {side, -side, side};
    vertices[6].pos = {-side, side, side};
    vertices[7].pos = {side, side, side};

    std::vector<int> indices = {
        0, 2, 1, 2, 3, 1,
        1, 3, 5, 3, 7, 5,
        2, 6, 3, 3, 6, 7,
        4, 5, 7, 4, 7, 6,
        0, 4, 2, 2, 4, 6,
        0, 1, 4, 1, 5, 4
    };

    pVertexBuffer_ = std::make_shared<VertexBuffer>(gfx, vertices);
    pIndexBuffer_ = std::make_shared<IndexBuffer>(gfx, indices);
    pTopology_ = Topology::TriangleList;
}

BlocksEngine::Mesh::Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer,
                         std::shared_ptr<Topology> topology)
    : pVertexBuffer_{std::move(vertexBuffer)},
      pIndexBuffer_{std::move(indexBuffer)},
      pTopology_{std::move(topology)}
{
}

void BlocksEngine::Mesh::Bind(const Graphics& gfx) noexcept
{
    pVertexBuffer_->Bind(gfx);
    pIndexBuffer_->Bind(gfx);
    pTopology_->Bind(gfx);
}

UINT BlocksEngine::Mesh::GetCount() const noexcept
{
    return pIndexBuffer_->GetCount();
}
