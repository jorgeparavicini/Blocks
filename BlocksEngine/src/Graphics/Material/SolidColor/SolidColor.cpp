#include "BlocksEngine/pch.h"
#include "BlocksEngine/SolidColor.h"


using namespace BlocksEngine;

std::vector<DirectX::XMVECTORF32> colors = {
    DirectX::Colors::Red,
    DirectX::Colors::Blue,
    DirectX::Colors::Cyan,
    DirectX::Colors::Green,
    DirectX::Colors::Yellow,
    DirectX::Colors::Magenta
};

DirectX::XMVECTORF32 colors2[6] = {
    DirectX::Colors::Red,
    DirectX::Colors::Blue,
    DirectX::Colors::Cyan,
    DirectX::Colors::Green,
    DirectX::Colors::Yellow,
    DirectX::Colors::Magenta
};


SolidColor::SolidColor(const Graphics& gfx)
    : Material{GetVertexShader(gfx), GetPixelShader(gfx), GetInputLayout(gfx)},
      pColorBuffer_{std::make_unique<PixelConstantBuffer<DirectX::XMVECTORF32[6]>>(gfx, colors2)}
{
}

std::shared_ptr<VertexShader> SolidColor::pVertexShader_ = nullptr;


std::shared_ptr<VertexShader> SolidColor::GetVertexShader(const Graphics& gfx)
{
    if (!pVertexShader_)
    {
        pVertexShader_ = std::make_shared<VertexShader>(gfx, L"resources/SolidColorVS.cso");
    }
    return pVertexShader_;
}

std::shared_ptr<PixelShader> SolidColor::pPixelShader_ = nullptr;

std::shared_ptr<PixelShader> SolidColor::GetPixelShader(const Graphics& gfx)
{
    if (!pPixelShader_)
    {
        pPixelShader_ = std::make_shared<PixelShader>(gfx, L"resources/SolidColorPS.cso");
    }
    return pPixelShader_;
}

std::shared_ptr<InputLayout> SolidColor::pInputLayout_ = nullptr;

std::shared_ptr<InputLayout> SolidColor::GetInputLayout(const Graphics& gfx)
{
    if (!pInputLayout_)
    {
        const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
            {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        pInputLayout_ = std::make_shared<InputLayout>(gfx, ied, GetVertexShader(gfx)->GetByteCode());
    }

    return pInputLayout_;
}

void SolidColor::Bind(const Graphics& gfx) noexcept
{
    pColorBuffer_->Bind(gfx);
    Material::Bind(gfx);
}
