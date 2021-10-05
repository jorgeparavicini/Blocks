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

#include "BlocksEngine/Bindable.h"
#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

namespace BlocksEngine
{
    class VertexBuffer;
}

class BlocksEngine::VertexBuffer final : public Bindable
{
public:
    template <class T>
    VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
        : stride_{sizeof T}
    {
        HRESULT hr;

        D3D11_BUFFER_DESC bd{};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = static_cast<UINT>(sizeof T * vertices.size());
        bd.StructureByteStride = sizeof T;

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertices.data();

        GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&bd, &sd, &pVertexBuffer_));
    }

    void Bind(const Graphics& gfx) noexcept override;

protected:
    UINT stride_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer_;
};
