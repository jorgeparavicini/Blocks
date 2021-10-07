// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Michael Paravicini
// File: Material.h

#pragma once
#include <memory>

#include "BlocksEngine/InputLayout.h"
#include "BlocksEngine/PixelShader.h"
#include "BlocksEngine/VertexShader.h"

namespace BlocksEngine
{
    class Material;
}

class BlocksEngine::Material : public Bindable
{
public:
    Material(std::shared_ptr<VertexShader> vertexShader,
             std::shared_ptr<PixelShader> pixelShader,
             std::shared_ptr<InputLayout> inputLayout);

    void Bind(const Graphics& gfx) noexcept override;


private:
    std::shared_ptr<VertexShader> pVertexShader_;
    std::shared_ptr<PixelShader> pPixelShader_;
    std::shared_ptr<InputLayout> pInputLayout_;
};
