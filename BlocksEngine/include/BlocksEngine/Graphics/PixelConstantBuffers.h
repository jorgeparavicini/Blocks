// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: PixelConstantBuffer.h

#pragma once

#include "BlocksEngine/Graphics/Material/ConstantBuffer.h"

namespace BlocksEngine
{
    template <typename T>
    class PixelConstantBuffer;
}

template <typename T>
class BlocksEngine::PixelConstantBuffer final : public ConstantBuffer<T>
{
public:
    using ConstantBuffer<T>::ConstantBuffer;

    void Bind(const Graphics& gfx) noexcept override
    {
        gfx.GetContext().PSSetConstantBuffers(this->slot_, 1u, this->pConstantBuffer_.GetAddressOf());
    }
};
