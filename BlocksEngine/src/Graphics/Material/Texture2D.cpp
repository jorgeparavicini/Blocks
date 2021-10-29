#include "BlocksEngine/pch.h"
#include "BlocksEngine/Texture2D.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"
#include "BlocksEngine/WICTextureLoader.h"

BlocksEngine::Texture2D::Texture2D(const Graphics& gfx, const std::wstring fileName)
{
    HRESULT hr;

    GFX_THROW_INFO(
        CreateWICTextureFromFile(&gfx.GetDevice(), &gfx.GetContext(), fileName.c_str(), nullptr, &pTextureView_));
}

void BlocksEngine::Texture2D::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().PSSetShaderResources(0u, 1u, pTextureView_.GetAddressOf());
}
