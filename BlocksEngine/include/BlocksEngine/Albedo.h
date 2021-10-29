// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Albedo.h

#pragma once
#include <DirectXMath.h>

#include "Material.h"
#include "PixelConstantBuffers.h"
#include "Sampler.h"
#include "Texture2D.h"

namespace BlocksEngine
{
    class Albedo;
}

class BlocksEngine::Albedo final : public Material
{
public:
    explicit Albedo(const Graphics& gfx, std::wstring textureFilename);

    static std::shared_ptr<VertexShader> GetVertexShader(const Graphics& gfx);
    static std::shared_ptr<PixelShader> GetPixelShader(const Graphics& gfx);
    static std::shared_ptr<InputLayout> GetInputLayout(const Graphics& gfx);

    void Bind(const Graphics& gfx) noexcept override;

private:
    static std::shared_ptr<VertexShader> pVertexShader_;
    static std::shared_ptr<PixelShader> pPixelShader_;
    static std::shared_ptr<InputLayout> pInputLayout_;

    std::unique_ptr<Texture2D> pTexture_;
    std::unique_ptr<PixelConstantBuffer<DirectX::XMVECTORF32>> pColor_;
    std::unique_ptr<Sampler> pSampler_;
};
