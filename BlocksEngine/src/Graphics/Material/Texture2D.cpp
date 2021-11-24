#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Material/Texture2D.h"

#include <DDSTextureLoader.h>

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/EngineException.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"

BlocksEngine::Texture2D::Texture2D(const Graphics& gfx, const std::wstring fileName)
{
    // HRESULT hr;

    throw ENGINE_EXCEPTION("Not implemented");
    //GFX_THROW_INFO(CreateWICTextureFromFile(&gfx.GetDevice(), &gfx.GetContext(), fileName.c_str(), nullptr, &pTextureView_));
}

BlocksEngine::Texture2D::Texture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView)
    : pTextureView_{std::move(textureView)}
{
}

std::unique_ptr<BlocksEngine::Texture2D> BlocksEngine::Texture2D::FromDds(
    const Graphics& gfx, const std::wstring fileName)
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;

    HRESULT hr;
    GFX_THROW_INFO(DirectX::CreateDDSTextureFromFile(&gfx.GetDevice(), fileName.c_str(), nullptr, &textureView));
    return std::make_unique<Texture2D>(std::move(textureView));
}

void BlocksEngine::Texture2D::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().PSSetShaderResources(0u, 1u, pTextureView_.GetAddressOf());
}
