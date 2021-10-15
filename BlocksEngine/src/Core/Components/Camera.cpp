#include "BlocksEngine/pch.h"
#include "BlocksEngine/Camera.h"

#include <boost/bind.hpp>

#include "BlocksEngine/Game.h"

using namespace BlocksEngine;

Camera::Camera(Actor& actor)
    : Component{actor},
      projection_{std::move(CalculateProjection())},
      windowResizedConnection_{
          GetWindow().AddSignalWindowResized(
              bind(&Camera::OnWindowResized, this, _1, _2))
      }
{
}

Matrix Camera::ViewProjection() const noexcept
{
    return View() * Projection();
}

Matrix Camera::View() const noexcept
{
    const Vector3 position = GetTransform().GetPosition();
    const Matrix rotation = Matrix::CreateFromQuaternion(GetTransform().GetRotation());

    Vector3 target = Vector3::Transform(Vector3::Forward, rotation);
    target.Normalize();

    return Matrix::CreateLookAt(position, position + target, Vector3::Up);
}

Matrix Camera::Projection() const noexcept
{
    return projection_;
}

void Camera::OnWindowResized(const int width, const int height) noexcept
{
    projection_ = std::move(CalculateProjection());
}

void Camera::Update()
{
    x_ += 0.01;
    //GetTransform().SetRotation(Quaternion::CreateFromYawPitchRoll(0, 1, x_));
    if (GetActor().GetGame().Keyboard().KeyIsPressed('W'))
    {
        GetTransform().GetPosition() += GetTransform().GetRotation() * Vector3::Forward * moveSpeed_;
    }

    if (GetActor().GetGame().Keyboard().KeyIsPressed('S'))
    {
        GetTransform().GetPosition() += GetTransform().GetRotation() * Vector3::Backward * moveSpeed_;
    }

    if (GetActor().GetGame().Keyboard().KeyIsPressed('A'))
    {
        GetTransform().GetPosition() += GetTransform().GetRotation() * Vector3::Left * moveSpeed_;
    }

    if (GetActor().GetGame().Keyboard().KeyIsPressed('D'))
    {
        GetTransform().GetPosition() += GetTransform().GetRotation() * Vector3::Right * moveSpeed_;
    }

    const Mouse& mouse = GetActor().GetGame().Mouse();
    float deltaX = mouse.GetPosX() - lastX_;
    float deltaY = mouse.GetPosY() - lastY_;
    lastX_ = mouse.GetPosX();
    lastY_ = mouse.GetPosY();

    rotation_.x = ClampAngle(rotation_.x + deltaY, -85.0f, 85.0f);
    rotation_.y += deltaX;
    GetTransform().SetRotation(Quaternion::Euler(rotation_));
}

float Camera::ClampAngle(float angle, const float min, const float max) const
{
    if (angle < -360.0f) angle += 360.0f;
    if (angle > 360.0f) angle -= 360.0f;
    return std::clamp(angle, min, max);
}

DirectX::XMMATRIX Camera::CalculateProjection() const noexcept
{
    return DirectX::XMMatrixPerspectiveFovLH(1.6f, GetActor().GetGraphics().AspectRatio(), 0.3f, 1000.0f);
}
