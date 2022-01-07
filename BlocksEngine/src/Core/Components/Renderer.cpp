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

    pConstantBuffer_ = std::make_shared<VertexConstantBuffer<CBPerObject>>(GetGame()->Graphics());
    if (pMaterial_)
    {
        // TODO: Maybe a copy needs to be created so not all references get the constant buffer
        pMaterial_->AddConstantBuffer(pConstantBuffer_);
    }

    if (!lightBuffer_)
    {
        Light light;
        light.dir = DirectX::XMFLOAT3{0.25f, -1.0f, 0.5f};
        light.ambient = DirectX::XMFLOAT4{0.4f, 0.4f, 0.4f, 1.0f};
        light.diffuse = DirectX::XMFLOAT4{0.6f, 0.6f, 0.6f, 1.0f};
        lightBuffer_ = std::make_unique<PixelConstantBuffer<Light>>(GetGame()->Graphics(), light);
    }
}

void Renderer::Draw()
{
    // TODO: Create default material to show a mesh when the material failed to load.
    if (!pMaterial_ || !pMesh_)
    {
        return;
    }

    const auto world = GetActor()->GetTransform()->GetMatrix();
    const auto wvp = world * GetGame()->MainCamera().WorldViewProjection();

    const Graphics& gfx = GetGame()->Graphics();

    CBPerObject cbPerObj;
    cbPerObj.world = XMMatrixTranspose(world);
    cbPerObj.wvp = XMMatrixTranspose(wvp);
    pConstantBuffer_->Update(gfx, cbPerObj);

    // TODO: This only needs to be done once per frame
    lightBuffer_->Bind(gfx);

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
