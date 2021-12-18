// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: IndexBuffer.h

#pragma once
#include <optional>
#include <vector>

#include "BlocksEngine/Graphics/Bindable.h"
#include "BlocksEngine/Graphics/Graphics.h"

namespace BlocksEngine
{
    class IndexBuffer;
}

class BlocksEngine::IndexBuffer final : public Bindable
{
public:
    IndexBuffer(const Graphics& gfx, UINT allocationSize, bool isStatic = true);

    IndexBuffer(const Graphics& gfx, const std::shared_ptr<void>& indices, size_t vertexSize, int vertexCount,
                bool isDynamic = false);

    IndexBuffer(const Graphics& gfx, const std::vector<int>& indices, UINT allocationSize = 0u,
                bool isStatic = true);

    void Bind(const Graphics& gfx) noexcept override;

    [[nodiscard]] UINT GetCount() const noexcept;
    [[nodiscard]] UINT GetSize() const noexcept;

    void Update(const Graphics& gfx, const std::vector<int>& indices) const;

protected:
    // Number of indices
    int count_;

    // Allocated size
    size_t size_;
    bool isStatic_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer_;

    void CreateBuffer(const Graphics& gfx, const std::vector<int>* indices = nullptr);
};
