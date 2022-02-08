// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: InputLayout.h

#pragma once
#include <vector>

#include "BlocksEngine/Graphics/Bindable.h"

namespace BlocksEngine
{
    class InputLayout;
}

class BlocksEngine::InputLayout final : public Bindable
{
public:
    InputLayout(const Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
                ID3DBlob* pVertexShaderByteCode);
    void Bind(const Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout_;
};
