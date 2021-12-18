#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/VertexBuffer.h"

BlocksEngine::VertexBuffer::VertexBuffer(const Graphics& gfx, const std::shared_ptr<void>& vertices,
                                         const size_t vertexSize, const size_t vertexCount, const bool isDynamic)
    : isDynamic_{isDynamic}, size_{static_cast<UINT>(vertexSize)}, count_{vertexCount}
{
    HRESULT hr;

    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = static_cast<UINT>(size_ * count_);
    bd.Usage = isDynamic_ ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0u;
    bd.StructureByteStride = size_;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices.get();

    GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&bd, &sd, &pVertexBuffer_));
}

size_t BlocksEngine::VertexBuffer::GetCount() const
{
    return count_;
}

void BlocksEngine::VertexBuffer::Bind(const Graphics& gfx) noexcept
{
    constexpr UINT offset = 0u;
    gfx.GetContext().IASetVertexBuffers(0u, 1u, pVertexBuffer_.GetAddressOf(), &size_, &offset);
}
