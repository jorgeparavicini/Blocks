// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Mesh.h

#pragma once
#include <memory>

#include "Vertex.h"
#include "BlocksEngine/Bindable.h"
#include "BlocksEngine/IndexBuffer.h"
#include "BlocksEngine/Topology.h"
#include "BlocksEngine/VertexBuffer.h"

namespace BlocksEngine
{
    class Mesh;
}

class BlocksEngine::Mesh final : public Bindable
{
public:
    explicit Mesh(const Graphics& gfx);

    Mesh(std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer,
         std::shared_ptr<Topology> topology = Topology::TriangleList);

    void Bind(const Graphics& gfx) noexcept override;
    [[nodiscard]] UINT GetCount() const noexcept;

private:
    std::shared_ptr<VertexBuffer> pVertexBuffer_;
    std::shared_ptr<IndexBuffer> pIndexBuffer_;
    std::shared_ptr<Topology> pTopology_;
};
