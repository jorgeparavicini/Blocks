// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: IndexBuffer.h

#pragma once
#include <vector>

#include "BlocksEngine/Bindable.h"
#include "BlocksEngine/Graphics.h"

namespace BlocksEngine
{
    class IndexBuffer;
}

class BlocksEngine::IndexBuffer final : public Bindable
{
public:
    IndexBuffer(const Graphics& gfx, const std::vector<unsigned short>& indices);
    void Bind(const Graphics& gfx) noexcept override;
    [[nodiscard]] UINT GetCount() const noexcept;

protected:
    UINT count_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer_;
};
