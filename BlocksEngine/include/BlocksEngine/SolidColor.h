// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: SolidColor.h

#pragma once
#include "PixelConstantBuffers.h"
#include "BlocksEngine/ConstantBuffer.h"
#include "BlocksEngine/Material.h"

namespace BlocksEngine
{
    class SolidColor;
}

class BlocksEngine::SolidColor final : public Material
{
public:
    explicit SolidColor(const Graphics& gfx);

    static std::shared_ptr<VertexShader> GetVertexShader(const Graphics& gfx);
    static std::shared_ptr<PixelShader> GetPixelShader(const Graphics& gfx);
    static std::shared_ptr<InputLayout> GetInputLayout(const Graphics& gfx);

    void Bind(const Graphics& gfx) noexcept override;

private:
    static std::shared_ptr<VertexShader> pVertexShader_;
    static std::shared_ptr<PixelShader> pPixelShader_;
    static std::shared_ptr<InputLayout> pInputLayout_;

    std::unique_ptr<PixelConstantBuffer<DirectX::XMVECTORF32[6]>> pColorBuffer_;
};
