// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Sampler.h

#pragma once

#include "BlocksEngine/Graphics/Bindable.h"

namespace BlocksEngine
{
    class Sampler;
}

class BlocksEngine::Sampler final : public Bindable
{
public:
    Sampler(const Graphics& gfx);

    void Bind(const Graphics& gfx) noexcept override;

protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler_;
};
