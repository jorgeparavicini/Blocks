#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Material/Texture2DArray.h"

#include <DDSTextureLoader.h>

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/EngineException.h"

using namespace BlocksEngine;

Texture2DArray::Texture2DArray(const Graphics& gfx, const std::vector<std::wstring>& fileNames)
{
    std::vector<ID3D11ShaderResourceView*> textures(fileNames.size());

    throw ENGINE_EXCEPTION("Not Implemented");
    //HRESULT hr;
    for (int i = 0; i < fileNames.size(); ++i)
    {
        //GFX_THROW_INFO(CreateWICTextureFromFile(&gfx.GetDevice(), &gfx.GetContext(), fileNames[i].c_str(), nullptr, &textures[i]));
    }

    ppTextureViews_ = std::move(textures);
}

Texture2DArray::~Texture2DArray()
{
    for (const auto ppTextureView : ppTextureViews_)
    {
        ppTextureView->Release();
    }
}

void Texture2DArray::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().PSSetShaderResources(0u, static_cast<UINT>(ppTextureViews_.size()), ppTextureViews_.data());
}
