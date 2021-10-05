#include "BlocksEngine/pch.h"
#include "BlocksEngine/Mesh.h"

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
