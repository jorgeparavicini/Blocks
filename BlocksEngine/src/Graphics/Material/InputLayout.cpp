#include "BlocksEngine/pch.h"
#include "BlocksEngine/InputLayout.h"

#include "BlocksEngine/DxgiInfoManager.h"
#include "BlocksEngine/GraphicsException.h"

BlocksEngine::InputLayout::InputLayout(const Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
                                       ID3DBlob* pVertexShaderByteCode)
{
    HRESULT hr;

    GFX_THROW_INFO(
        gfx.GetDevice().CreateInputLayout(
            layout.data(),
            static_cast<UINT>(layout.size()),
            pVertexShaderByteCode->GetBufferPointer(),
            pVertexShaderByteCode->GetBufferSize(),
            &pInputLayout_));
}

void BlocksEngine::InputLayout::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().IASetInputLayout(pInputLayout_.Get());
}
