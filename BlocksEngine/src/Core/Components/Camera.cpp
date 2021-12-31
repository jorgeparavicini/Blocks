#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Camera.h"

#include <boost/bind.hpp>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Matrix Camera::WorldViewProjection() const noexcept
{
    return wvp_;
}

Matrix Camera::WorldView() const noexcept
{
    const Vector3 position = GetTransform()->GetPosition();
    const Matrix rotation = Matrix::CreateFromQuaternion(GetTransform()->GetOrientation());

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
    worldView_ = WorldView();
    wvp_ = worldView_ * projection_;

    /*const auto deltaTime = static_cast<float>(GetGame()->Time().DeltaTime());

    //bool isGrounded = physx::
    const auto orientation = GetTransform()->GetOrientation();

    physx::PxVec3 movement{physx::PxZero};

    if (keyboard.KeyIsPressed('W'))
    {
        movement += orientation * Vector3<float>::Forward;
    }
    if (keyboard.KeyIsPressed('S'))
    {
        movement += orientation * Vector3<float>::Backward;
    }
    if (keyboard.KeyIsPressed('A'))
    {
        movement += orientation * Vector3<float>::Up;
    }
    if (keyboard.KeyIsPressed('D'))
    {
        movement += orientation * Vector3<float>::Down;
    }

    controller_->move(movement * deltaTime * moveSpeed_, 0.0f, deltaTime, physx::PxControllerFilters{});
    velocity_.y += GetGame()->GetPhysics().GetScene().getGravity().y * deltaTime;
    controller_->move(velocity_ * deltaTime, 0.0f, deltaTime, physx::PxControllerFilters{});

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

    const auto newOrientation =
        Quaternion::CreateFromAxisAngle(Vector3<float>::Up, deltaX * rotationSpeed_ * deltaTime);
    GetTransform()->SetOrientation(orientation * newOrientation);*/
}

void Camera::Start()
{
    SetEventTypes(EventType::Update);

    windowResizedConnection_ = GetGame()->Window().AddSignalWindowResized(
        bind(&Camera::OnWindowResized, this, _1, _2)
    );
    projection_ = CalculateProjection();
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
