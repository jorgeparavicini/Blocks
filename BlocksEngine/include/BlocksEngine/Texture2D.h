// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Texture2D.h

#pragma once
#include "BlocksEngine/Bindable.h"

namespace BlocksEngine
{
    class Texture2D;
}

class BlocksEngine::Texture2D final : public Bindable
{
public:
    Texture2D(const Graphics& gfx, std::wstring fileName);
    Texture2D(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView);

    void Bind(const Graphics& gfx) noexcept override;

    static std::unique_ptr<Texture2D> FromDds(const Graphics& gfx, std::wstring fileName);

protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView_;
};
