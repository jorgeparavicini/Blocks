#include "BlocksEngine/pch.h"
#include "BlocksEngine/IndexBuffer.h"

#include "BlocksEngine/BufferException.h"
#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

// TODO: THIS REALLY NEEDS SOME UNIT TESTING

BlocksEngine::IndexBuffer::IndexBuffer(const Graphics& gfx, const UINT allocationSize, const bool isStatic)
    : count_{0},
      size_{allocationSize},
      isStatic_{isStatic}
{
    CreateBuffer(gfx);
}

BlocksEngine::IndexBuffer::IndexBuffer(const Graphics& gfx, const std::vector<int>& indices,
                                       const UINT allocationSize, const bool isStatic)
    : count_{static_cast<UINT>(indices.size())},
      size_{allocationSize == 0 ? static_cast<UINT>(count_ * sizeof(int)) : allocationSize},
      isStatic_{isStatic}
{
    CreateBuffer(gfx, &indices);
}

void BlocksEngine::IndexBuffer::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().IASetIndexBuffer(pIndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0u);
}

UINT BlocksEngine::IndexBuffer::GetCount() const noexcept
{
    return count_;
}

UINT BlocksEngine::IndexBuffer::GetSize() const noexcept
{
    return size_;
}

// TODO: Also needs some serious testing
void BlocksEngine::IndexBuffer::Update(const Graphics& gfx, const std::vector<int>& indices) const
{
    D3D11_MAPPED_SUBRESOURCE isd;
    gfx.GetContext().Map(pIndexBuffer_.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &isd);
    memcpy(isd.pData, indices.data(), indices.size());
    gfx.GetContext().Unmap(pIndexBuffer_.Get(), 0u);
}

void BlocksEngine::IndexBuffer::CreateBuffer(const Graphics& gfx,
                                             const std::vector<int>* const indices)
{
    if (indices && indices->size() * sizeof(int) > size_)
    {
        throw BUFFER_EXCEPTION("Size of data is larger than allocation size");
    }

    HRESULT hr;

    D3D11_BUFFER_DESC ibd{};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = isStatic_ ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
    ibd.CPUAccessFlags = isStatic_ ? 0u : D3D11_CPU_ACCESS_WRITE;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = size_;
    ibd.StructureByteStride = sizeof(int);

    std::optional<D3D11_SUBRESOURCE_DATA> data = std::nullopt;

    if (indices)
    {
        D3D11_SUBRESOURCE_DATA isd{};
        isd.pSysMem = indices->data();
        data = isd;
    }

    GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&ibd, &data.value(), &pIndexBuffer_));
}
