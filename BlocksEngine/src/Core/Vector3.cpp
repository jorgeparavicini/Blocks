#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector3.h"

using BlocksEngine::Vector3;

Vector3::Vector3(const float x, const float y, const float z)
    : x_{x}, y_{y}, z_{z}
{
}

float Vector3::X() const noexcept
{
    return x_;
}

float Vector3::Y() const noexcept
{
    return y_;
}

float Vector3::Z() const noexcept
{
    return z_;
}

float Vector3::Magnitude() const noexcept
{
    return sqrt(SqrMagnitude());
}

float Vector3::SqrMagnitude() const noexcept
{
    return x_ * x_ + y_ * y_ + z_ * z_;
}

Vector3 Vector3::Normalized() const noexcept
{
    const float magnitude = Magnitude();
    return Vector3(x_ / magnitude, y_ / magnitude, z_ / magnitude);
}

float Vector3::operator[](const int i) const
{
    switch (i)
    {
    case 0: return x_;
    case 1: return y_;
    case 2: return z_;
    default: throw std::out_of_range("The index is out of range");
    }
}

Vector3 Vector3::One()
{
    return Vector3(1, 1, 1);
}

Vector3 Vector3::Zero()
{
    return Vector3(0, 0, 0);
}

Vector3 Vector3::Back()
{
    return Vector3(0, 0, -1);
}

Vector3 Vector3::Down()
{
    return Vector3(0, -1, 0);
}

Vector3 Vector3::Forward()
{
    return Vector3(0, 0, 1);
}

Vector3 Vector3::Left()
{
    return Vector3(-1, 0, 0);
}

Vector3 Vector3::Right()
{
    return Vector3(1, 0, 0);
}

Vector3 Vector3::Up()
{
    return Vector3(0, 1, 0);
}
