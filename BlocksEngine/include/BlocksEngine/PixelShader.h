// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: PixelShader.h

#pragma once
#include <string>

#include "BlocksEngine/Bindable.h"
#include "BlocksEngine/Graphics.h"

namespace BlocksEngine
{
    class PixelShader;
}

class BlocksEngine::PixelShader final : public Bindable
{
public:
    PixelShader(const Graphics& gfx, const std::wstring& path);
    void Bind(const Graphics& gfx) noexcept override;

    static std::shared_ptr<PixelShader> SolidColor(const Graphics& gfx);

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader_;

private:
    static std::shared_ptr<PixelShader> pSolidColor_;
};
