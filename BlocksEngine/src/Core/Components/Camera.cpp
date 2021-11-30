#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Camera.h"

#include <boost/bind.hpp>

#include "BlocksEngine/Core/Input/Keyboard.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Camera::Camera(std::weak_ptr<Actor> actor)
    : Component{std::move(actor)},
      projection_{std::move(CalculateProjection())},
      windowResizedConnection_{
          GetGame()->Window().AddSignalWindowResized(
              // ReSharper disable CppRedundantQualifier
              bind(&Camera::OnWindowResized, this, boost::placeholders::_1, boost::placeholders::_2))
          // ReSharper restore CppRedundantQualifier
      }
{
}

Matrix Camera::ViewProjection() const noexcept
{
    return WorldView() * Projection();
}

Matrix Camera::WorldView() const noexcept
{
    const Vector3 position = GetTransform()->GetPosition();
    const Matrix rotation = Matrix::CreateFromQuaternion(GetTransform()->GetRotation());

    Vector3 target = Vector3<float>::Transform(Vector3<float>::Forward, rotation);
    target.Normalize();

    return Matrix::CreateLookAt(position, position + target, Vector3<float>::Up);
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
    const auto deltaTime = static_cast<float>(GetGame()->Time().DeltaTime());
    x_ += 0.01f;
    const Keyboard& keyboard = GetActor()->GetGame()->Keyboard();
    //GetTransform()->SetRotation(Quaternion::CreateFromYawPitchRoll(0, 1, x_));
    if (keyboard.KeyIsPressed('W'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Forward * moveSpeed_ *
            deltaTime;
    }

    if (keyboard.KeyIsPressed('S'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Backward * moveSpeed_ *
            deltaTime;
    }

    if (keyboard.KeyIsPressed('A'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Left * moveSpeed_ * deltaTime;
    }

    if (keyboard.KeyIsPressed('D'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Right * moveSpeed_ * deltaTime;
    }

    if (keyboard.KeyIsPressed('Q'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Up * moveSpeed_ * deltaTime;
    }

    if (keyboard.KeyIsPressed('E'))
    {
        GetTransform()->GetPosition() += GetTransform()->GetRotation() * Vector3<float>::Down * moveSpeed_ * deltaTime;
    }

    const Mouse& mouse = GetActor()->GetGame()->Mouse();
    //float deltaX = mouse.GetPosX() - lastX_;
    //float deltaY = mouse.GetPosY() - lastY_;
    float deltaX = 0;
    float deltaY = 0;

    if (keyboard.KeyIsPressed(VK_LEFT))
    {
        deltaX -= keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_RIGHT))
    {
        deltaX += keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_UP))
    {
        deltaY -= keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_DOWN))
    {
        deltaY += keyboardRotationSpeed_;
    }

    lastX_ = static_cast<float>(mouse.GetPosX());
    lastY_ = static_cast<float>(mouse.GetPosY());

    rotation_.x = ClampAngle(rotation_.x + deltaY * rotationSpeed_ * deltaTime, -85.0f, 85.0f);
    rotation_.y += deltaX * rotationSpeed_ * deltaTime;
    GetTransform()->SetRotation(Quaternion::Euler(rotation_));
}

float Camera::ClampAngle(float angle, const float min, const float max) const
{
    if (angle < -360.0f) angle += 360.0f;
    if (angle > 360.0f) angle -= 360.0f;
    return std::clamp(angle, min, max);
}

DirectX::XMMATRIX Camera::CalculateProjection() const noexcept
{
    return DirectX::XMMatrixPerspectiveFovLH(1.0f, GetGame()->Graphics().AspectRatio(), 0.3f, 1000.0f);
}
