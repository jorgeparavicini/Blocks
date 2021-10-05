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

#include "Bindable.h"
#include "Graphics.h"

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

    static std::shared_ptr<VertexShader> SolidColor(const Graphics& gfx);


protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader_;

private:
    static std::shared_ptr<VertexShader> pSolidColor_;
};
