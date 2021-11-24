// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: VertexShader.h

#pragma once
#include <string>

#include "BlocksEngine/Graphics/Bindable.h"
#include "BlocksEngine/Graphics/Graphics.h"

namespace BlocksEngine
{
    class VertexShader;
}

class BlocksEngine::VertexShader final : public Bindable
{
public:
    VertexShader(const Graphics& gfx, const std::wstring& path);
    void Bind(const Graphics& gfx) noexcept override;
    [[nodiscard]] ID3DBlob* GetByteCode() const noexcept;

protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader_;
};
