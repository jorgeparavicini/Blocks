// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Texture2DArray.h

#pragma once

#include "BlocksEngine/Graphics/Bindable.h"

namespace BlocksEngine
{
    class Texture2DArray;
}

class BlocksEngine::Texture2DArray final : public Bindable
{
public:
    Texture2DArray(const Graphics& gfx, const std::vector<std::wstring>& fileNames);
    ~Texture2DArray() override;

    void Bind(const Graphics& gfx) noexcept override;

protected:
    std::vector<ID3D11ShaderResourceView*> ppTextureViews_;
};
