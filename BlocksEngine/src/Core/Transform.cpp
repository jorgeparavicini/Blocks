#include "BlocksEngine/pch.h"
#include "BlocksEngine/Transform.h"

using namespace DirectX;

BlocksEngine::Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale)
    : position_{position}, rotation_{rotation}, scale_{scale}
{
}

XMMATRIX BlocksEngine::Transform::GetMatrix() const
{
    return (XMMatrixTranslation(position_.x, position_.y, position_.z)
            + XMMatrixScaling(scale_.x, scale_.y, scale_.z))
        * XMMatrixRotationX(rotation_.x)
        * XMMatrixRotationY(rotation_.y)
        * XMMatrixRotationZ(rotation_.z);
}

BlocksEngine::Vector3 BlocksEngine::Transform::GetPosition() const noexcept
{
    return position_;
}

void BlocksEngine::Transform::SetPosition(const Vector3 position) noexcept
{
    position_ = position;
}
