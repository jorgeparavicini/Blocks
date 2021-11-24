#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Material/VertexShader.h"

#include <d3dcompiler.h>

#include "BlocksEngine/DebugUtility/DxgiInfoManager.h"
#include "BlocksEngine/Exceptions/GraphicsException.h"

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
