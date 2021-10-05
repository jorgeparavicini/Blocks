#include "BlocksEngine/pch.h"
#include "BlocksEngine/IndexBuffer.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

BlocksEngine::IndexBuffer::IndexBuffer(const Graphics& gfx, const std::vector<unsigned short>& indices)
    : count_{static_cast<UINT>(indices.size())}
{
    HRESULT hr;

    D3D11_BUFFER_DESC ibd{};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = static_cast<UINT>(count_ * sizeof(unsigned short));
    ibd.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA isd{};
    isd.pSysMem = indices.data();

    GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&ibd, &isd, &pIndexBuffer_));
}

void BlocksEngine::IndexBuffer::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().IASetIndexBuffer(pIndexBuffer_.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT BlocksEngine::IndexBuffer::GetCount() const noexcept
{
    return count_;
}
