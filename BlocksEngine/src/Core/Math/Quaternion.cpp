#include "BlocksEngine/pch.h"
#include "BlocksEngine/Quaternion.h"

#include <sstream>

#include "BlocksEngine/Math.h"

using namespace BlocksEngine;
using namespace DirectX;

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

Quaternion::Quaternion() noexcept
    : XMFLOAT4{0, 0, 0, 1.0f}
{
}

constexpr Quaternion::Quaternion(const float ix, const float iy, const float iz, const float iw) noexcept
    : XMFLOAT4{ix, iy, iz, iw}
{
}

Quaternion::Quaternion(const Vector3& v, const float scalar) noexcept
    : XMFLOAT4{v.x, v.y, v.z, scalar}
{
}

Quaternion::Quaternion(const Vector4& v) noexcept
    : XMFLOAT4{v.x, v.y, v.z, v.w}
{
}

Quaternion::Quaternion(_In_reads_(4) const float* pArray) noexcept
    : XMFLOAT4{pArray}
{
}

Quaternion::Quaternion(FXMVECTOR v) noexcept
    : XMFLOAT4{}
{
    XMStoreFloat4(this, v);
}

Quaternion::Quaternion(const XMFLOAT4& q) noexcept
    : XMFLOAT4{q.x, q.y, q.z, q.w}
{
}

Quaternion::Quaternion(const XMVECTORF32& f) noexcept
    : XMFLOAT4{f.f[0], f.f[1], f.f[2], f.f[3]}
{
}

//------------------------------------------------------------------------------
// Convertor operators
//------------------------------------------------------------------------------

Quaternion::operator DirectX::XMVECTOR() const noexcept
{
    return XMLoadFloat4(this);
}

Quaternion::operator std::string() const noexcept
{
    std::ostringstream oss;
    oss << "{" << x << ", " << y << ", " << z << ", " << w << "}" << std::flush;
    return oss.str();
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Quaternion::operator==(const Quaternion& q) const noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    return XMQuaternionEqual(q1, q2);
}

bool Quaternion::operator!=(const Quaternion& q) const noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    return XMQuaternionNotEqual(q1, q2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Quaternion& Quaternion::operator=(const XMVECTORF32& f) noexcept
{
    x = f.f[0];
    y = f.f[1];
    z = f.f[2];
    w = f.f[3];

    return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& q) noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMVectorAdd(q1, q2));

    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& q) noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMVectorSubtract(q1, q2));

    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q) noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));

    return *this;
}

Quaternion& Quaternion::operator*=(const float s) noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMVectorScale(q, s));

    return *this;
}

Quaternion& Quaternion::operator/=(const Quaternion& q) noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    XMVECTOR q2 = XMLoadFloat4(&q);
    q2 = XMQuaternionInverse(q2);
    XMStoreFloat4(this, XMQuaternionMultiply(q1, q2));

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

Quaternion Quaternion::operator+() const noexcept
{
    return *this;
}

Quaternion Quaternion::operator-() const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);

    Quaternion r;
    XMStoreFloat4(&r, XMVectorNegate(q));
    return r;
}

//------------------------------------------------------------------------------
// Quaternion operations
//------------------------------------------------------------------------------

float Quaternion::Length() const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    return XMVectorGetX(XMQuaternionLength(q));
}

float Quaternion::LengthSquared() const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    return XMVectorGetX(XMQuaternionLengthSq(q));
}

void Quaternion::Normalize() noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMQuaternionNormalize(q));
}

void Quaternion::Normalize(Quaternion& result) const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionNormalize(q));
}

void Quaternion::Conjugate() noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(this, XMQuaternionConjugate(q));
}

void Quaternion::Conjugate(Quaternion& result) const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionConjugate(q));
}

void Quaternion::Inverse(Quaternion& result) const noexcept
{
    const XMVECTOR q = XMLoadFloat4(this);
    XMStoreFloat4(&result, XMQuaternionInverse(q));
}

float Quaternion::Dot(const Quaternion& q) const noexcept
{
    const XMVECTOR q1 = XMLoadFloat4(this);
    const XMVECTOR q2 = XMLoadFloat4(&q);
    return XMVectorGetX(XMQuaternionDot(q1, q2));
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, const float angle) noexcept
{
    const XMVECTOR a = XMLoadFloat3(&axis);

    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionRotationAxis(a, angle));
    return r;
}

Quaternion Quaternion::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll) noexcept
{
    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    return r;
}

Quaternion Quaternion::CreateFromYawPitchRoll(const Vector3& rot) noexcept
{
    const XMVECTOR v = XMLoadFloat3(&rot);

    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionRotationRollPitchYawFromVector(v));
    return r;
}

Quaternion Quaternion::CreateFromRotationMatrix(const Matrix& m) noexcept
{
    const XMMATRIX m0 = XMLoadFloat4x4(&m);

    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionRotationMatrix(m0));
    return r;
}

void Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float t, Quaternion& result) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    const XMVECTOR dot = XMVector4Dot(quat1, quat2);

    XMVECTOR r;
    if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
    {
        r = XMVectorLerp(quat1, quat2, t);
    }
    else
    {
        const XMVECTOR tv = XMVectorReplicate(t);
        const XMVECTOR t1V = XMVectorReplicate(1.f - t);
        const XMVECTOR x0 = XMVectorMultiply(quat1, t1V);
        const XMVECTOR x1 = XMVectorMultiply(quat2, tv);
        r = XMVectorSubtract(x0, x1);
    }

    XMStoreFloat4(&result, XMQuaternionNormalize(r));
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float t) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    XMVECTOR dot = XMVector4Dot(quat1, quat2);

    XMVECTOR r;
    if (XMVector4GreaterOrEqual(dot, XMVectorZero()))
    {
        r = XMVectorLerp(quat1, quat2, t);
    }
    else
    {
        XMVECTOR tv = XMVectorReplicate(t);
        XMVECTOR t1V = XMVectorReplicate(1.f - t);
        XMVECTOR x0 = XMVectorMultiply(quat1, t1V);
        XMVECTOR x1 = XMVectorMultiply(quat2, tv);
        r = XMVectorSubtract(x0, x1);
    }

    Quaternion result;
    XMStoreFloat4(&result, XMQuaternionNormalize(r));
    return result;
}

void Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float t, Quaternion& result) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);
    XMStoreFloat4(&result, XMQuaternionSlerp(quat1, quat2, t));
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float t) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    Quaternion result;
    XMStoreFloat4(&result, XMQuaternionSlerp(quat1, quat2, t));
    return result;
}

void Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);
    XMStoreFloat4(&result, XMQuaternionMultiply(quat2, quat1));
}

Quaternion Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    Quaternion result;
    XMStoreFloat4(&result, XMQuaternionMultiply(quat2, quat1));
    return result;
}

Quaternion Quaternion::EulerRadians(const Vector3& v)
{
    return EulerRadians(v.x, v.y, v.z);
}

Quaternion Quaternion::EulerRadians(const float x, const float y, const float z)
{
    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionRotationRollPitchYaw(x, y, z));
    return r;
    /*const float roll = z * 0.5f;
    const float sinRoll = std::sin(roll);
    const float cosRoll = std::cos(roll);

    const float pitch = y * 0.5f;
    const float sinPitch = std::sin(pitch);
    const float cosPitch = std::cos(pitch);

    const float yaw = x * 0.5f;
    const float sinYaw = std::sin(yaw);
    const float cosYaw = std::cos(yaw);

    const float resX = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
    const float resY = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * sinRoll;
    const float resZ = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
    const float resW = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
    return {resX, resY, resZ, resW};*/
}

Quaternion Quaternion::Euler(const Vector3& v)
{
    return EulerRadians(v * Math::DegToRad);
}

Quaternion Quaternion::Euler(const float x, const float y, const float z)
{
    return EulerRadians(Vector3(x, y, z) * Math::DegToRad);
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const Quaternion Quaternion::Identity = {0.f, 0.f, 0.f, 1.f};


//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Quaternion operator+(const Quaternion& q1, const Quaternion& q2) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    Quaternion r;
    XMStoreFloat4(&r, XMVectorAdd(quat1, quat2));
    return r;
}

Quaternion operator-(const Quaternion& q1, const Quaternion& q2) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    Quaternion r;
    XMStoreFloat4(&r, XMVectorSubtract(quat1, quat2));
    return r;
}

Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    const XMVECTOR quat2 = XMLoadFloat4(&q2);

    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionMultiply(quat1, quat2));
    return r;
}

Quaternion operator*(const Quaternion& q, const float s) noexcept
{
    const XMVECTOR quat = XMLoadFloat4(&q);

    Quaternion r;
    XMStoreFloat4(&r, XMVectorScale(quat, s));
    return r;
}

Quaternion operator/(const Quaternion& q1, const Quaternion& q2) noexcept
{
    const XMVECTOR quat1 = XMLoadFloat4(&q1);
    XMVECTOR quat2 = XMLoadFloat4(&q2);
    quat2 = XMQuaternionInverse(quat2);

    Quaternion r;
    XMStoreFloat4(&r, XMQuaternionMultiply(quat1, quat2));
    return r;
}

Quaternion operator*(const float s, const Quaternion& q) noexcept
{
    const XMVECTOR quat = XMLoadFloat4(&q);

    Quaternion r;
    XMStoreFloat4(&r, XMVectorScale(quat, s));
    return r;
}
