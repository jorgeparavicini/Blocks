#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector3.h"

using BlocksEngine::Vector3;

Vector3::Vector3(const float x, const float y, const float z)
    : x{x}, y{y}, z{z}
{
}

float Vector3::Magnitude() const noexcept
{
    return sqrt(SqrMagnitude());
}

float Vector3::SqrMagnitude() const noexcept
{
    return x * x + y * y + z * z;
}

Vector3 Vector3::Normalized() const noexcept
{
    const float magnitude = Magnitude();
    return Vector3(x / magnitude, y / magnitude, z / magnitude);
}

float Vector3::operator[](const int i) const
{
    switch (i)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
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
