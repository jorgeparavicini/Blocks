#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Material/Sampler.h"

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"

BlocksEngine::Sampler::Sampler(const Graphics& gfx)
{
    HRESULT hr;

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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
