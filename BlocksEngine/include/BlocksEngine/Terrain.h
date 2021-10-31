﻿// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Terrain.h

#pragma once
#include "Material.h"
#include "Sampler.h"
#include "Texture2D.h"
#include "Texture2DArray.h"

namespace BlocksEngine
{
    class Terrain;
}

class BlocksEngine::Terrain final : public Material
{
public:
    explicit Terrain(const Graphics& gfx, const std::vector<std::wstring>& textureNames);

    static std::shared_ptr<VertexShader> GetVertexShader(const Graphics& gfx) noexcept;
    static std::shared_ptr<PixelShader> GetPixelShader(const Graphics& gfx) noexcept;
    static std::shared_ptr<InputLayout> GetInputLayout(const Graphics& gfx) noexcept;

    void Bind(const Graphics& gfx) noexcept override;

private:
    static std::shared_ptr<VertexShader> pVertexShader_;
    static std::shared_ptr<PixelShader> pPixelShader_;
    static std::shared_ptr<InputLayout> pInputLayout_;

    std::unique_ptr<Texture2DArray> pTextures_;
    std::unique_ptr<Sampler> pSampler_;
};
