﻿// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Renderer.h

#pragma once

#include "BlocksEngine/Component.h"
#include "BlocksEngine/Material.h"
#include "BlocksEngine/Mesh.h"
#include "BlocksEngine/VertexConstantBuffer.h"

namespace BlocksEngine
{
    class Renderer;
}

class BlocksEngine::Renderer final : public Component
{
public:
    explicit Renderer(Actor& actor);
    Renderer(Actor& actor, std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh);

    void SetEnabled(bool enabled) noexcept;
    [[nodiscard]] bool IsEnabled() const noexcept;

    void Draw() override;

private:
    bool enabled_{true};
    std::shared_ptr<Material> pMaterial_;
    std::shared_ptr<Mesh> pMesh_;
    std::shared_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pConstantBuffer_;
};