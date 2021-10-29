﻿#include "BlocksEngine/pch.h"
#include "BlocksEngine/Material.h"

BlocksEngine::Material::Material(std::shared_ptr<VertexShader> vertexShader,
                                 std::shared_ptr<PixelShader> pixelShader,
                                 std::shared_ptr<InputLayout> inputLayout)
    : pVertexShader_{std::move(vertexShader)},
      pPixelShader_{std::move(pixelShader)},
      pInputLayout_{std::move(inputLayout)}
{
}

void BlocksEngine::Material::Bind(const Graphics& gfx) noexcept
{
    for (const auto& constantBuffer : constantBuffers_)
    {
        constantBuffer->Bind(gfx);
    }
    pVertexShader_->Bind(gfx);
    pPixelShader_->Bind(gfx);
    pInputLayout_->Bind(gfx);
}

void BlocksEngine::Material::AddConstantBuffer(std::shared_ptr<ConstantBufferBase> constantBuffer)
{
    constantBuffers_.push_back(std::move(constantBuffer));
}
