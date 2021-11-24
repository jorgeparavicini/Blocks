#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Renderer.h"

#include <DirectXMath.h>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Graphics/Material/SolidColor/SolidColor.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Renderer::Renderer(const std::weak_ptr<Actor> actor)
    : Renderer{
        actor,
        std::make_shared<SolidColor>(actor.lock()->GetGame()->Graphics()),
        std::make_shared<Mesh>(actor.lock()->GetGame()->Graphics())
    }
{
}

// TODO: These constructors could be optimized with move semantics
Renderer::Renderer(const std::weak_ptr<Actor> actor, std::shared_ptr<Material> pMaterial, std::shared_ptr<Mesh> pMesh)
    : Component{actor},
      pMaterial_{std::move(pMaterial)},
      pMesh_{std::move(pMesh)},
      pConstantBuffer_{std::make_shared<VertexConstantBuffer<DirectX::XMMATRIX>>(actor.lock()->GetGame()->Graphics())}
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

    const auto wvp = GetActor()->GetTransform()->GetMatrix() * GetGame()->MainCamera().ViewProjection();

    const Graphics& gfx = GetGame()->Graphics();

    pConstantBuffer_->Update(gfx, XMMatrixTranspose(wvp));

    pMaterial_->Bind(gfx);
    pMesh_->Bind(gfx);
    gfx.GetContext().DrawIndexed(pMesh_->GetCount(), 0, 0);
}
