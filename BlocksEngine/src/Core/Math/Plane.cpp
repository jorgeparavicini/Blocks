#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Math/Plane.h"

using namespace BlocksEngine;
using namespace DirectX;

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

Plane::Plane() noexcept
    : XMFLOAT4{0.0f, 1.0f, 0.0f, 0.0f}
{
}

constexpr Plane::Plane(const float ix, const float iy, const float iz, const float iw) noexcept
    : XMFLOAT4{ix, iy, iz, iw}
{
}

Plane::Plane(const Vector3<float>& normal, const float d) noexcept
    : XMFLOAT4{normal.x, normal.y, normal.z, d}
{
}

Plane::Plane(const Vector3<float>& point1, const Vector3<float>& point2, const Vector3<float>& point3) noexcept
    : XMFLOAT4{}
{
    const XMVECTOR p0 = XMLoadFloat3(&point1);
    const XMVECTOR p1 = XMLoadFloat3(&point2);
    const XMVECTOR p2 = XMLoadFloat3(&point3);
    XMStoreFloat4(this, XMPlaneFromPoints(p0, p1, p2));
}

Plane::Plane(const Vector3<float>& point, const Vector3<float>& normal) noexcept
    : XMFLOAT4{}
{
    const XMVECTOR p = XMLoadFloat3(&point);
    const XMVECTOR n = XMLoadFloat3(&normal);
    XMStoreFloat4(this, XMPlaneFromPointNormal(p, n));
}

Plane::Plane(const Vector4<float>& v) noexcept
    : XMFLOAT4{v.x, v.y, v.z, v.w}
{
}

Plane::Plane(_In_reads_(4) const float* pArray) noexcept
    : XMFLOAT4{pArray}
{
}

Plane::Plane(FXMVECTOR v) noexcept
    : XMFLOAT4{}
{
    XMStoreFloat4(this, v);
}

Plane::Plane(const XMFLOAT4& p) noexcept
    : XMFLOAT4{p.x, p.y, p.z, p.w}
{
}

Plane::Plane(const XMVECTORF32& f) noexcept
    : XMFLOAT4{f.f[0], f.f[1], f.f[2], f.f[3]}
{
}

//------------------------------------------------------------------------------
// State manipulation
//------------------------------------------------------------------------------

bool Plane::IsDirty() const noexcept
{
    return isDirty_;
}

void Plane::ConsumeIsDirty() noexcept
{
    isDirty_ = false;
}

//------------------------------------------------------------------------------
// Convertor operators
//------------------------------------------------------------------------------

Plane::operator DirectX::XMVECTOR() const noexcept
{
    return XMLoadFloat4(this);
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Plane::operator==(const Plane& p) const noexcept
{
    const XMVECTOR p1 = XMLoadFloat4(this);
    const XMVECTOR p2 = XMLoadFloat4(&p);
    return XMPlaneEqual(p1, p2);
}

bool Plane::operator!=(const Plane& p) const noexcept
{
    const XMVECTOR p1 = XMLoadFloat4(this);
    const XMVECTOR p2 = XMLoadFloat4(&p);
    return XMPlaneNotEqual(p1, p2);
}


//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Plane& Plane::operator=(const XMVECTORF32& f) noexcept
{
    x = f.f[0];
    y = f.f[1];
    z = f.f[2];
    w = f.f[3];

    isDirty_ = true;

    return *this;
}

//------------------------------------------------------------------------------
// Plane properties
//------------------------------------------------------------------------------

Vector3<float> Plane::Normal() const noexcept
{
    return Vector3(x, y, z);
}

void Plane::Normal(const Vector3<float>& normal) noexcept
{
    x = normal.x;
    y = normal.y;
    z = normal.z;

    isDirty_ = true;
}

float Plane::D() const noexcept
{
    return w;
}

void Plane::D(const float d) noexcept
{
    w = d;

    isDirty_ = true;
}

//------------------------------------------------------------------------------
// Plane operations
//------------------------------------------------------------------------------

void Plane::Normalize() noexcept
{
    const XMVECTOR p = XMLoadFloat4(this);
    XMStoreFloat4(this, XMPlaneNormalize(p));

    isDirty_ = true;
}

void Plane::Normalize(Plane& result) const noexcept
{
    const XMVECTOR p = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMPlaneNormalize(p));
}

float Plane::Dot(const Vector4<float>& v) const noexcept
{
    const XMVECTOR p = XMLoadFloat4(this);
    const XMVECTOR v0 = XMLoadFloat4(&v);
    return XMVectorGetX(XMPlaneDot(p, v0));
}

float Plane::DotCoordinate(const Vector3<float>& position) const noexcept
{
    const XMVECTOR p = XMLoadFloat4(this);
    const XMVECTOR v0 = XMLoadFloat3(&position);
    return XMVectorGetX(XMPlaneDotCoord(p, v0));
}

float Plane::DotNormal(const Vector3<float>& normal) const noexcept
{
    const XMVECTOR p = XMLoadFloat4(this);
    const XMVECTOR n0 = XMLoadFloat3(&normal);
    return XMVectorGetX(XMPlaneDotNormal(p, n0));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

void Plane::Transform(const Plane& plane, const Matrix& m, Plane& result) noexcept
{
    const XMVECTOR p = XMLoadFloat4(&plane);
    const XMMATRIX m0 = XMLoadFloat4x4(&m);
    XMStoreFloat4(&result, XMPlaneTransform(p, m0));
}

Plane Plane::Transform(const Plane& plane, const Matrix& m) noexcept
{
    const XMVECTOR p = XMLoadFloat4(&plane);
    const XMMATRIX m0 = XMLoadFloat4x4(&m);

    Plane result;
    XMStoreFloat4(&result, XMPlaneTransform(p, m0));
    return result;
}

void Plane::Transform(const Plane& plane, const Quaternion& rotation, Plane& result) noexcept
{
    const XMVECTOR p = XMLoadFloat4(&plane);
    const XMVECTOR q = XMLoadFloat4(&rotation);
    XMVECTOR x = XMVector3Rotate(p, q);
    x = XMVectorSelect(p, x, g_XMSelect1110); // result.d = plane.d
    XMStoreFloat4(&result, x);
}

Plane Plane::Transform(const Plane& plane, const Quaternion& rotation) noexcept
{
    const XMVECTOR p = XMLoadFloat4(&plane);
    const XMVECTOR q = XMLoadFloat4(&rotation);
    XMVECTOR x = XMVector3Rotate(p, q);
    x = XMVectorSelect(p, x, g_XMSelect1110); // result.d = plane.d

    Plane result;
    XMStoreFloat4(&result, x);
    return result;
}
