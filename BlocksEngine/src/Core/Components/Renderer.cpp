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
        std::make_shared<SolidColor>(actor.GetGame().Graphics()),
        std::make_shared<Mesh>(actor.GetGame().Graphics())
    }
{
}

Renderer::Renderer(Actor& actor, std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh)
    : Component{actor},
      pMaterial_{std::move(pMaterial)},
      pMesh_{std::move(pMesh)},
      pConstantBuffer_{std::make_shared<VertexConstantBuffer<DirectX::XMMATRIX>>(actor.GetGame().Graphics())}
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

    const auto wvp = GetActor().GetTransform().GetMatrix() * GetGame().MainCamera().ViewProjection();

    const Graphics& gfx = GetGame().Graphics();

    pConstantBuffer_->Update(gfx, XMMatrixTranspose(wvp));

    pMaterial_->Bind(gfx);
    pMesh_->Bind(gfx);
    gfx.GetContext().DrawIndexed(pMesh_->GetCount(), 0, 0);
}
