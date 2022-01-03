#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Renderer.h"

#include <DirectXMath.h>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Graphics/Material/SolidColor/SolidColor.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Renderer::Renderer()
    : Renderer{nullptr, nullptr}
{
}

Renderer::Renderer(std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh)
    : pMaterial_{std::move(pMaterial)},
      pMesh_{std::move(pMesh)}
{
}

void Renderer::Start()
{
    SetEventTypes(EventType::Render);

    pConstantBuffer_ = std::make_shared<VertexConstantBuffer<DirectX::XMMATRIX>>(GetGame()->Graphics());
    if (pMaterial_)
    {
        // TODO: Maybe a copy needs to be created so not all references get the constant buffer
        pMaterial_->AddConstantBuffer(pConstantBuffer_);
    }
}

void Renderer::Draw()
{
    // TODO: Create default material to show a mesh when the material failed to load.
    if (!pMaterial_ || !pMesh_)
    {
        return;
    }
    const auto t = GetActor()->GetTransform()->GetMatrix();
    const auto wvp = GetActor()->GetTransform()->GetMatrix() * GetGame()->MainCamera().WorldViewProjection();

    const Graphics& gfx = GetGame()->Graphics();

    pConstantBuffer_->Update(gfx, XMMatrixTranspose(wvp));

    pMaterial_->Bind(gfx);
    pMesh_->Bind(gfx);
    gfx.GetContext().DrawIndexed(pMesh_->GetCount(), 0, 0);
}

void Renderer::SetMesh(std::shared_ptr<Mesh> mesh)
{
    pMesh_ = std::move(mesh);
}

void Renderer::SetMaterial(std::shared_ptr<Material> material)
{
    pMaterial_ = std::move(material);
    // TODO: This is not ideal at all
    pMaterial_->AddConstantBuffer(pConstantBuffer_);
}
