#include "BlocksEngine/pch.h"
#include "BlocksEngine/Transform.h"

using namespace DirectX;

BlocksEngine::Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale)
    : position_{position}, rotation_{rotation}, scale_{scale}
{
}

XMMATRIX BlocksEngine::Transform::GetMatrix() const
{
    return (XMMatrixTranslation(position_.X(), position_.Y(), position_.Z())
            + XMMatrixScaling(scale_.X(), scale_.Y(), scale_.Z()))
        * XMMatrixRotationX(rotation_.X())
        * XMMatrixRotationY(rotation_.Y())
        * XMMatrixRotationZ(rotation_.Z());
}

void BlocksEngine::Transform::SetPosition(const Vector3 position) noexcept
{
    position_ = position;
}
