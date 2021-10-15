#include "BlocksEngine/pch.h"
#include "BlocksEngine/Renderer.h"

#include <DirectXMath.h>

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Game.h"
#include "BlocksEngine/SolidColor.h"

using namespace BlocksEngine;

Renderer::Renderer(Actor& actor)
    : Renderer{
        actor,
        std::make_shared<SolidColor>(actor.GetGraphics()),
        std::make_shared<Mesh>(actor.GetGraphics())
    }
{
}

Renderer::Renderer(Actor& actor, std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh)
    : Component{actor},
      pMaterial_{std::move(pMaterial)},
      pMesh_{std::move(pMesh)},
      pConstantBuffer_{std::make_shared<VertexConstantBuffer<DirectX::XMMATRIX>>(actor.GetGraphics())}
{
    pMaterial_->AddConstantBuffer(pConstantBuffer_);
}

void Renderer::SetEnabled(const bool enabled) noexcept
{
    enabled_ = enabled;
}

bool Renderer::IsEnabled() const noexcept
{
    return enabled_;
}

void Renderer::Draw()
{
    if (!pMaterial_ || !pMesh_)
    {
        return;
    }

    const auto wvp = GetActor().GetTransform().GetMatrix() * GetActor().GetGame().GetCamera().ViewProjection();

    pConstantBuffer_->Update(GetActor().GetGraphics(), XMMatrixTranspose(wvp));

    pMaterial_->Bind(GetActor().GetGraphics());
    pMesh_->Bind(GetActor().GetGraphics());
    GetActor().GetGraphics().GetContext().DrawIndexed(pMesh_->GetCount(), 0, 0);
}
