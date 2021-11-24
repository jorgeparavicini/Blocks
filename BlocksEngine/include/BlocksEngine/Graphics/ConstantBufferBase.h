// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: ConstantBufferBase.h

#pragma once
#include "BlocksEngine/Graphics/Bindable.h"

namespace BlocksEngine
{
    class ConstantBufferBase;
}


class BlocksEngine::ConstantBufferBase : public Bindable
{
public:
    explicit ConstantBufferBase(const UINT slot = 0)
        : slot_{slot}
    {
    }

protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer_{};
    UINT slot_;
};
