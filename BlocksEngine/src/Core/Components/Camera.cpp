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
