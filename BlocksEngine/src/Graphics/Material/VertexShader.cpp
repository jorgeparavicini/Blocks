#include "BlocksEngine/pch.h"
#include "BlocksEngine/VertexShader.h"

#include <d3dcompiler.h>

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

BlocksEngine::VertexShader::VertexShader(const Graphics& gfx, const std::wstring& path)
{
    HRESULT hr;
    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pByteCodeBlob_));
    GFX_THROW_INFO(gfx.GetDevice().CreateVertexShader(
        pByteCodeBlob_->GetBufferPointer(),
        pByteCodeBlob_->GetBufferSize(),
        nullptr,
        &pVertexShader_));
}

void BlocksEngine::VertexShader::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().VSSetShader(pVertexShader_.Get(), nullptr, 0u);
}

ID3DBlob* BlocksEngine::VertexShader::GetByteCode() const noexcept
{
    return pByteCodeBlob_.Get();
}

std::shared_ptr<BlocksEngine::VertexShader> BlocksEngine::VertexShader::pSolidColor_ = nullptr;

std::shared_ptr<BlocksEngine::VertexShader> BlocksEngine::VertexShader::SolidColor(const Graphics& gfx)
{
    if (!pSolidColor_)
    {
        pSolidColor_ = std::make_shared<VertexShader>(gfx, L"resources/SolidColorVS.cso");
    }
    return pSolidColor_;
}
