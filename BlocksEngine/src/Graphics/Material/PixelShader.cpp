#include "BlocksEngine/pch.h"
#include "BlocksEngine/PixelShader.h"

#include <d3dcompiler.h>

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

#pragma comment(lib, "D3DCompiler.lib")

BlocksEngine::PixelShader::PixelShader(const Graphics& gfx, const std::wstring& path)
{
    HRESULT hr;

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
    GFX_THROW_INFO(
        gfx.GetDevice().CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader_));
}

void BlocksEngine::PixelShader::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().PSSetShader(pPixelShader_.Get(), nullptr, 0u);
}
