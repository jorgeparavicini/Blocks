// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: VertexBuffer.h

#pragma once
#include <vector>

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"
#include "BlocksEngine/Graphics/Bindable.h"

namespace BlocksEngine
{
    class VertexBuffer;
}

class BlocksEngine::VertexBuffer final : public Bindable
{
public:
    // TODO: Optional allocation size

    VertexBuffer(const Graphics& gfx, const std::shared_ptr<void>& vertices, size_t vertexSize, size_t vertexCount,
                 bool isDynamic = false);

    template <class T>
    VertexBuffer(const Graphics& gfx, const std::vector<T>& vertices, bool isDynamic);

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] bool IsDynamic() const;

    [[nodiscard]] size_t GetSize() const;

    [[nodiscard]] size_t GetCount() const;


    template <class T>
    void Update(const Graphics& gfx, const std::vector<T>& vertices);

    void Bind(const Graphics& gfx) noexcept override;

protected:
    bool isDynamic_;
    UINT size_;
    size_t count_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer_;
};

template <class T>
BlocksEngine::VertexBuffer::VertexBuffer(const Graphics& gfx, const std::vector<T>& vertices, const bool isDynamic)
    : isDynamic_{isDynamic}, size_{sizeof T}, count_{vertices.size()}
{
    HRESULT hr;

    D3D11_BUFFER_DESC bd;
    bd.ByteWidth = static_cast<UINT>(size_ * count_);
    bd.Usage = isDynamic_ ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.MiscFlags = 0u;
    bd.StructureByteStride = size_;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertices.data();

    GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&bd, &sd, &pVertexBuffer_));
}

template <class T>
void BlocksEngine::VertexBuffer::Update(const Graphics& gfx, const std::vector<T>& vertices)
{
    // TODO: Theoretically if is dynamic is disabled we could still allow updating by just creating a new vertex buffer.
    assert("Only dynamic buffers can be updated" && isDynamic_);

    // TODO: Create a larger buffer if the existing one is too small
    // TODO: If the new data is smaller, it probably leaves some old data intact. Could be bugprone???????????
    D3D11_MAPPED_SUBRESOURCE resource;
    gfx.GetContext().Map(pVertexBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, vertices.data(), vertices.size() * sizeof(T));
    gfx.GetContext().Unmap(pVertexBuffer_.Get(), 0);
}

inline bool BlocksEngine::VertexBuffer::IsDynamic() const
{
    return isDynamic_;
}

inline size_t BlocksEngine::VertexBuffer::GetSize() const
{
    return size_;
}
