#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Renderer.h"

#include <DirectXMath.h>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Graphics/Material/SolidColor/SolidColor.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Renderer::Renderer(std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh)
    : material_{std::move(pMaterial)},
      mesh_{std::move(pMesh)}
{
    ListenToMeshLayoutChanges();
}

void Renderer::Start()
{
    SetEventTypes(EventType::Render);

    wvpBuffer_ = std::make_shared<VertexConstantBuffer<DirectX::XMMATRIX>>(GetGame()->Graphics());
}

void Renderer::Draw()
{
    // TODO: Create default material to show a mesh when the material failed to load.
    if (!material_ || !mesh_)
    {
        return;
    }

    const Graphics& gfx = GetGame()->Graphics();

    if (mesh_->RequiresUpload())
    {
        mesh_->Upload(gfx);
    }

    const auto wvp = GetActor()->GetTransform()->GetMatrix() * GetGame()->MainCamera().WorldViewProjection();


    wvpBuffer_->Update(gfx, XMMatrixTranspose(wvp));
    wvpBuffer_->Bind(gfx);

    material_->Bind(gfx);
    mesh_->Bind(gfx);
    gfx.GetContext().DrawIndexed(mesh_->GetVertexCount(), 0, 0);
}

void Renderer::SetMesh(std::shared_ptr<Mesh> mesh)
{
    mesh_ = std::move(mesh);
    if (meshChangesConnection_)
    {
        meshChangesConnection_->disconnect();
    }
    ListenToMeshLayoutChanges();
}

void Renderer::SetMaterial(std::shared_ptr<Material> material)
{
    material_ = std::move(material);
}
