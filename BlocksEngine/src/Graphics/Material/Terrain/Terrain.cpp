#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Material/Terrain/Terrain.h"

#include "BlocksEngine/Graphics/Material/Albedo/Albedo.h"

using namespace BlocksEngine;

Terrain::Terrain(const Graphics& gfx, std::shared_ptr<Texture2D> texture)
    : Material{GetVertexShader(gfx), GetPixelShader(gfx), GetInputLayout(gfx)},
      pTexture_{std::move(texture)},
      pSampler_{std::make_unique<Sampler>(gfx)}
{
}

std::shared_ptr<VertexShader> Terrain::pVertexShader_ = nullptr;

std::shared_ptr<VertexShader> Terrain::GetVertexShader(const Graphics& gfx) noexcept
{
    if (!pVertexShader_)
    {
        pVertexShader_ = std::make_shared<VertexShader>(gfx, L"resources/TerrainVS.cso");
    }
    return pVertexShader_;
}

std::shared_ptr<PixelShader> Terrain::pPixelShader_ = nullptr;

std::shared_ptr<PixelShader> Terrain::GetPixelShader(const Graphics& gfx) noexcept
{
    if (!pPixelShader_)
    {
        pPixelShader_ = std::make_shared<PixelShader>(gfx, L"resources/TerrainPS.cso");
    }
    return pPixelShader_;
}

std::shared_ptr<InputLayout> Terrain::pInputLayout_ = nullptr;

std::shared_ptr<InputLayout> Terrain::GetInputLayout(const Graphics& gfx) noexcept
{
    if (!pInputLayout_)
    {
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TexIndex", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        pInputLayout_ = std::make_shared<InputLayout>(gfx, ied, GetVertexShader(gfx)->GetByteCode());
    }

    return pInputLayout_;
}

void Terrain::Bind(const Graphics& gfx) noexcept
{
    pSampler_->Bind(gfx);
    pTexture_->Bind(gfx);
    Material::Bind(gfx);
}
