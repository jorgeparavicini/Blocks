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
#include "BlocksEngine/Main/Game.h"

namespace BlocksEngine
{
    class Renderer;
}

class BlocksEngine::Renderer final : public Component
{
public:
    explicit Renderer() = default;
    Renderer(std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh);

    void Start() override;
    void Draw() override;

    void SetMesh(std::shared_ptr<Mesh> mesh);
    void SetMaterial(std::shared_ptr<Material> material);

private:
    std::shared_ptr<Material> material_;
    std::shared_ptr<Mesh> mesh_;
    std::shared_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> wvpBuffer_;

    std::shared_ptr<InputLayout> inputLayout_;

    std::optional<boost::signals2::connection> meshChangesConnection_;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void ListenToMeshLayoutChanges();
};

inline void BlocksEngine::Renderer::ListenToMeshLayoutChanges()
{
    meshChangesConnection_ = mesh_->AddSignalVertexLayoutChanged(
        [this](const std::vector<VertexAttributeDescriptor>& attrs)
        {
            std::vector<D3D11_INPUT_ELEMENT_DESC> ied{};
            for (auto& vertexAttributeDescriptor : attrs)
            {
                ied.push_back(vertexAttributeDescriptor);
            }
            inputLayout_ = std::make_shared<InputLayout>(GetGame()->Graphics(), ied, material_->GetShaderBlob());
        });
}
