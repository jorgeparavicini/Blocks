#include "BlocksEngine/pch.h"
#include "BlocksEngine/Sampler.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

BlocksEngine::Sampler::Sampler(const Graphics& gfx)
{
    HRESULT hr;

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = -FLT_MAX;
    samplerDesc.MaxLOD = FLT_MAX;

    GFX_THROW_INFO(gfx.GetDevice().CreateSamplerState(&samplerDesc, &pSampler_));
}

void BlocksEngine::Sampler::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().PSSetSamplers(0, 1, pSampler_.GetAddressOf());
}
