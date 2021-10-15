#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector4.h"

using namespace DirectX;
using namespace BlocksEngine;


//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

Vector4::Vector4() noexcept
    : XMFLOAT4(0.f, 0.f, 0.f, 0.f)
{
}

constexpr Vector4::Vector4(const float a) noexcept
    : XMFLOAT4(a, a, a, a)
{
}

constexpr Vector4::Vector4(const float x, const float y, const float z, const float w) noexcept
    : XMFLOAT4(x, y, z, w)
{
}

Vector4::Vector4(_In_reads_(4) const float* pArray) noexcept
    : XMFLOAT4(pArray)
{
}

Vector4::Vector4(FXMVECTOR v) noexcept
    : XMFLOAT4()
{
    XMStoreFloat4(this, v);
}

Vector4::Vector4(const XMFLOAT4& v) noexcept
    : XMFLOAT4{v.x, v.y, v.z, v.w}
{
}

Vector4::Vector4(const XMVECTORF32& f) noexcept
    : XMFLOAT4(f.f[0], f.f[1], f.f[2], f.f[3])
{
}

//------------------------------------------------------------------------------
// Convertor operators
//------------------------------------------------------------------------------

Vector4::operator DirectX::XMVECTOR() const noexcept
{
    return XMLoadFloat4(this);
}

//------------------------------------------------------------------------------
// State manipulation
//------------------------------------------------------------------------------

bool Vector4::IsDirty() const noexcept
{
    return isDirty_;
}

void Vector4::ConsumeIsDirty() noexcept
{
    isDirty_ = false;
}


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Vector4::operator==(const Vector4& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    return XMVector4Equal(v1, v2);
}

bool Vector4::operator!=(const Vector4& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Vector4& Vector4::operator=(const XMVECTORF32& f) noexcept
{
    x = f.f[0];
    y = f.f[1];
    z = f.f[2];
    w = f.f[3];

    isDirty_ = true;

    return *this;
}

Vector4& Vector4::operator+=(const Vector4& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorAdd(v1, v2);

    XMStoreFloat4(this, x);

    isDirty_ = true;

    return *this;
}

Vector4& Vector4::operator-=(const Vector4& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorSubtract(v1, v2);

    XMStoreFloat4(this, x);

    isDirty_ = true;

    return *this;
}

Vector4& Vector4::operator*=(const Vector4& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorMultiply(v1, v2);

    XMStoreFloat4(this, x);

    isDirty_ = true;

    return *this;
}

Vector4& Vector4::operator*=(const float s) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVectorScale(v1, s);

    XMStoreFloat4(this, x);

    isDirty_ = true;

    return *this;
}

Vector4& Vector4::operator/=(const float s) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVectorScale(v1, 1.0f / s);

    XMStoreFloat4(this, x);

    isDirty_ = true;

    return *this;
}


//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

Vector4 Vector4::operator+() const noexcept
{
    return *this;
}

Vector4 Vector4::operator-() const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVectorNegate(v1);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}


//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool Vector4::InBounds(const Vector4& bounds) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&bounds);

    return XMVector4InBounds(v1, v2);
}

float Vector4::Length() const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVector4Length(v1);
    return XMVectorGetX(x);
}

float Vector4::LengthSquared() const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVector4LengthSq(v1);
    return XMVectorGetX(x);
}

float Vector4::Dot(const Vector4& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVector4Dot(v1, v2);
    return XMVectorGetX(x);
}

void Vector4::Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(this);
    const XMVECTOR x2 = XMLoadFloat4(&v1);
    const XMVECTOR x3 = XMLoadFloat4(&v2);
    const XMVECTOR r = XMVector4Cross(x1, x2, x3);

    XMStoreFloat4(&result, r);
}

Vector4 Vector4::Cross(const Vector4& v1, const Vector4& v2) const noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(this);
    const XMVECTOR x2 = XMLoadFloat4(&v1);
    const XMVECTOR x3 = XMLoadFloat4(&v2);
    const XMVECTOR r = XMVector4Cross(x1, x2, x3);

    Vector4 result;
    XMStoreFloat4(&result, r);
    return result;
}

void Vector4::Normalize() noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVector4Normalize(v1);

    XMStoreFloat4(this, x);

    isDirty_ = true;
}

void Vector4::Normalize(Vector4& result) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR x = XMVector4Normalize(v1);

    XMStoreFloat4(&result, x);
}

void Vector4::Clamp(const Vector4& vMin, const Vector4& vMax) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&vMin);
    const XMVECTOR v3 = XMLoadFloat4(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);

    XMStoreFloat4(this, x);

    isDirty_ = true;
}

void Vector4::Clamp(const Vector4& vMin, const Vector4& vMax, Vector4& result) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(this);
    const XMVECTOR v2 = XMLoadFloat4(&vMin);
    const XMVECTOR v3 = XMLoadFloat4(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);

    XMStoreFloat4(&result, x);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float Vector4::Distance(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector4Length(v);

    return XMVectorGetX(x);
}

float Vector4::DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector4LengthSq(v);

    return XMVectorGetX(x);
}

void Vector4::Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Min(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Max(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Lerp(const Vector4& v1, const Vector4& v2, const float t, Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, const float f, const float g,
                          Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x3 = XMLoadFloat4(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, const float f,
                             const float g) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x3 = XMLoadFloat4(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, const float t,
                         Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x3 = XMLoadFloat4(&v3);
    const XMVECTOR x4 = XMLoadFloat4(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4,
                            const float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x3 = XMLoadFloat4(&v3);
    const XMVECTOR x4 = XMLoadFloat4(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, const float t,
                      Vector4& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&t1);
    const XMVECTOR x3 = XMLoadFloat4(&v2);
    const XMVECTOR x4 = XMLoadFloat4(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2,
                         const float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&t1);
    const XMVECTOR x3 = XMLoadFloat4(&v2);
    const XMVECTOR x4 = XMLoadFloat4(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Reflect(const Vector4& iVec, const Vector4& nVec, Vector4& result) noexcept
{
    const XMVECTOR i = XMLoadFloat4(&iVec);
    const XMVECTOR n = XMLoadFloat4(&nVec);
    const XMVECTOR x = XMVector4Reflect(i, n);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Reflect(const Vector4& iVec, const Vector4& nVec) noexcept
{
    const XMVECTOR i = XMLoadFloat4(&iVec);
    const XMVECTOR n = XMLoadFloat4(&nVec);
    const XMVECTOR x = XMVector4Reflect(i, n);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Refract(const Vector4& iVec, const Vector4& nVec, const float refractionIndex, Vector4& result) noexcept
{
    const XMVECTOR i = XMLoadFloat4(&iVec);
    const XMVECTOR n = XMLoadFloat4(&nVec);
    const XMVECTOR x = XMVector4Refract(i, n, refractionIndex);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Refract(const Vector4& iVec, const Vector4& nVec, const float refractionIndex) noexcept
{
    const XMVECTOR i = XMLoadFloat4(&iVec);
    const XMVECTOR n = XMLoadFloat4(&nVec);
    const XMVECTOR x = XMVector4Refract(i, n, refractionIndex);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Transform(const Vector2& v, const Quaternion& quat) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat3(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Transform(const Vector3& v, const Quaternion& quat) noexcept
{
    const XMVECTOR v1 = XMLoadFloat3(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(v1, x, g_XMSelect1110); // result.w = v.w

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Transform(const Vector4& v, const Quaternion& quat) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    XMVECTOR x = XMVector3Rotate(v1, q);
    x = XMVectorSelect(v1, x, g_XMSelect1110); // result.w = v.w

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

void Vector4::Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector4Transform(v1, m1);

    XMStoreFloat4(&result, x);
}

Vector4 Vector4::Transform(const Vector4& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = XMLoadFloat4(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector4Transform(v1, m1);

    Vector4 result;
    XMStoreFloat4(&result, x);
    return result;
}

_Use_decl_annotations_

void Vector4::Transform(const Vector4* varray, const size_t count, const Matrix& m, Vector4* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, m1);
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const Vector4 Vector4::Zero = {0.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::One = {1.f, 1.f, 1.f, 1.f};
const Vector4 Vector4::UnitX = {1.f, 0.f, 0.f, 0.f};
const Vector4 Vector4::UnitY = {0.f, 1.f, 0.f, 0.f};
const Vector4 Vector4::UnitZ = {0.f, 0.f, 1.f, 0.f};
const Vector4 Vector4::UnitW = {0.f, 0.f, 0.f, 1.f};


//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

Vector4 operator+(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorAdd(x1, x2);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator-(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorSubtract(x1, x2);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator*(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorMultiply(x1, x2);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator*(const Vector4& v, const float s) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator/(const Vector4& v1, const Vector4& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v1);
    const XMVECTOR x2 = XMLoadFloat4(&v2);
    const XMVECTOR x = XMVectorDivide(x1, x2);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator/(const Vector4& v, const float s) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorScale(x1, 1.0f / s);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}

Vector4 operator*(const float s, const Vector4& v) noexcept
{
    const XMVECTOR x1 = XMLoadFloat4(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    Vector4 r;
    XMStoreFloat4(&r, x);
    return r;
}
