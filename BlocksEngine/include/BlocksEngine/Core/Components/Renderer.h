// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Renderer.h

#pragma once

#include <DirectXMath.h>

#include "BlocksEngine/Core/Components/Component.h"
#include "BlocksEngine/Graphics/VertexConstantBuffer.h"
#include "BlocksEngine/Graphics/Material/Material.h"
#include "BlocksEngine/Graphics/Mesh/Mesh.h"

namespace BlocksEngine
{
    class Renderer;
}

class BlocksEngine::Renderer final : public Component
{
public:
    explicit Renderer(ComponentArgs args);
    Renderer(ComponentArgs args, std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh);

    void SetEnabled(bool enabled) noexcept;
    [[nodiscard]] bool IsEnabled() const noexcept;

    void Draw() override;

    void SetMesh(std::shared_ptr<Mesh> mesh);
    void SetMaterial(std::shared_ptr<Material> material);

private:
    bool enabled_{true};
    std::shared_ptr<Material> pMaterial_;
    std::shared_ptr<Mesh> pMesh_;
    std::shared_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pConstantBuffer_;
};
