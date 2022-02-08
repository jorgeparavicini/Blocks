#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/VertexBuffer.h"

void BlocksEngine::VertexBuffer::Bind(const Graphics& gfx) noexcept
{
    constexpr UINT offset = 0u;
    gfx.GetContext().IASetVertexBuffers(0u, 1u, pVertexBuffer_.GetAddressOf(), &stride_, &offset);
}
