#include "BlocksEngine/pch.h"
#include "BlocksEngine/Camera.h"

#include <boost/bind.hpp>

BlocksEngine::Camera::Camera(Actor& actor)
    : Component{actor},
      projection_{DirectX::XMMatrixPerspectiveLH(1.0f, 1 / GetActor().GetGraphics().AspectRatio(), 0.5f, 40.0f)},
      windowResizedConnection_{
          GetWindow().AddSignalWindowResized(
              bind(&Camera::OnWindowResized, this, _1, _2))
      }
{
}

DirectX::XMMATRIX BlocksEngine::Camera::Projection() const noexcept
{
    return projection_;
}

void BlocksEngine::Camera::OnWindowResized(const int width, const int height) noexcept
{
    projection_ = DirectX::XMMatrixPerspectiveLH(1.0f, static_cast<float>(height) / static_cast<float>(width), 0.5f,
                                                 40.0f);
}

void BlocksEngine::Camera::Update()
{
    Vector3 pos = GetTransform().GetPosition();
    pos.x += 0.01f;
    pos.y += 0.01f;
    GetTransform().SetPosition(pos);
}
