#include "BlocksEngine/pch.h"
#include "BlocksEngine/Camera.h"

BlocksEngine::Camera::Camera(Actor& actor)
    : Component{actor},
      projection_{DirectX::XMMatrixPerspectiveLH(1.0f, GetActor().GetGraphics().AspectRatio(), 0.5f, 40.0f)}
{
}

DirectX::XMMATRIX BlocksEngine::Camera::Projection() const noexcept
{
    return projection_;
}
