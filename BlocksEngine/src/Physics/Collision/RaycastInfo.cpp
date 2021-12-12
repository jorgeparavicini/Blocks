#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/RaycastInfo.h"

BlocksEngine::RaycastInfo::RaycastInfo()
    : hitFraction{0.0f}, meshSubpart{-1}, triangleIndex{-1}, body{nullptr}, collider{nullptr}
{
}

BlocksEngine::RaycastTest::RaycastTest(std::unique_ptr<RaycastCallback> callback)
    : userCallback{std::move(callback)}
{
}

float BlocksEngine::RaycastTest::RaycastAgainstShape(const Collider& shape, const Ray& ray)
{
    RaycastInfo info;

    if (shape.Raycast(ray, info))
    {
        return userCallback->NotifyRaycastHit(info);
    }

    return ray.maxFraction;
}
