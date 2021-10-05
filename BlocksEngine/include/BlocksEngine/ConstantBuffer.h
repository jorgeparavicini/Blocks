// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConstantBuffer.h

#pragma once

#include "BlocksEngine/Bindable.h"
#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

namespace BlocksEngine
{
    template <typename T>
    class ConstantBuffer;
}

template <typename T>
class BlocksEngine::ConstantBuffer : public Bindable
{
public:
    ConstantBuffer(const Graphics& gfx, const T& constants, const UINT slot = 0u)
        : slot_{slot}
    {
        HRESULT hr;

        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC; // TODO: Constant Buffers should not always be dynamic
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof constants;
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA csd{};
        csd.pSysMem = &constants;
        GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&cbd, &csd, &pConstantBuffer_));
    }

    explicit ConstantBuffer(const Graphics& gfx, const UINT slot = 0u)
        : slot_{slot}
    {
        HRESULT hr;

        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC; // TODO: Constant Buffers should not always be dynamic
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof T;
        cbd.StructureByteStride = 0u;

        GFX_THROW_INFO(gfx.GetDevice().CreateBuffer(&cbd, nullptr, &pConstantBuffer_));
    }

    void Update(const Graphics& gfx, const T& constants)
    {
        HRESULT hr;
        D3D11_MAPPED_SUBRESOURCE msr;
        GFX_THROW_INFO(gfx.GetContext().Map(pConstantBuffer_.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
    }


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer_;
    UINT slot_;
};
