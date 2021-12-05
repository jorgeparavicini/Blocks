#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Math/Ray.h"

using namespace BlocksEngine;

Ray::Ray(const Vector3<>& p1, const Vector3<>& p2, const float maxFraction)
    : point1{p1}, point2{p2}, maxFraction{maxFraction}
{
}

Ray::Ray(const Ray& ray)
    : point1{ray.point1}, point2{ray.point2}, maxFraction{ray.maxFraction}
{
}

Ray& Ray::operator=(const Ray& ray)
{
    if (&ray != this)
    {
        point1 = ray.point1;
        point2 = ray.point2;
        maxFraction = ray.maxFraction;
    }

    return *this;
}

Ray::Ray(Ray&& ray) noexcept
    : point1{std::exchange(ray.point1, Vector3<float>::Zero)},
      point2{std::exchange(ray.point2, Vector3<float>::Zero)},
      maxFraction{std::exchange(ray.maxFraction, 0)}
{
}

Ray& Ray::operator=(Ray&& ray) noexcept
{
    if (&ray != this)
    {
        point1 = std::exchange(ray.point1, Vector3<float>::Zero);
        point2 = std::exchange(ray.point2, Vector3<float>::Zero);
        maxFraction = std::exchange(ray.maxFraction, 0);
    }

    return *this;
}
