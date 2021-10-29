#include "BlocksEngine/pch.h"
#include "BlocksEngine/Albedo.h"

#include "BlocksEngine/Sampler.h"
#include "BlocksEngine/WICTextureLoader.h"

using namespace BlocksEngine;

Albedo::Albedo(const Graphics& gfx, std::wstring textureFilename)
    : Material{GetVertexShader(gfx), GetPixelShader(gfx), GetInputLayout(gfx)},
      pTexture_{std::make_unique<Texture2D>(gfx, std::move(textureFilename))},
      pSampler_{std::make_unique<Sampler>(gfx)}
{
}

std::shared_ptr<VertexShader> Albedo::pVertexShader_ = nullptr;

std::shared_ptr<VertexShader> Albedo::GetVertexShader(const Graphics& gfx)
{
    if (!pVertexShader_)
    {
        pVertexShader_ = std::make_shared<VertexShader>(gfx, L"resources/AlbedoVS.cso");
    }
    return pVertexShader_;
}

std::shared_ptr<PixelShader> Albedo::pPixelShader_ = nullptr;

std::shared_ptr<PixelShader> Albedo::GetPixelShader(const Graphics& gfx)
{
    if (!pPixelShader_)
    {
        pPixelShader_ = std::make_shared<PixelShader>(gfx, L"resources/AlbedoPS.cso");
    }
    return pPixelShader_;
}

std::shared_ptr<InputLayout> Albedo::pInputLayout_ = nullptr;

std::shared_ptr<InputLayout> Albedo::GetInputLayout(const Graphics& gfx)
{
    if (!pInputLayout_)
    {
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        pInputLayout_ = std::make_shared<InputLayout>(gfx, ied, GetVertexShader(gfx)->GetByteCode());
    }

    return pInputLayout_;
}

void Albedo::Bind(const Graphics& gfx) noexcept
{
    //pColor_->Bind(gfx);
    pSampler_->Bind(gfx);
    pTexture_->Bind(gfx);
    Material::Bind(gfx);
}
