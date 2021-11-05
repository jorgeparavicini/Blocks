#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector2.h"

#include "BlocksEngine/Quaternion.h"

using namespace BlocksEngine;
using namespace DirectX;

Vector2<int> x = Vector2<int>(2);

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

template <class T>
Vector2<T>::Vector2() noexcept
    : Vector2Base<T>::Base{0, 0}
{
}

template <class T>
constexpr Vector2<T>::Vector2(const T a) noexcept
    : Vector2Base<T>::Base{a, a}
{
}

template <class T>
constexpr Vector2<T>::Vector2(const T ix, const T iy) noexcept
    : Vector2Base<T>::Base{ix, iy}
{
}

template <class T>
Vector2<T>::Vector2(_In_reads_(2) const T* pArray) noexcept
    : Vector2Base<T>::Base{pArray}
{
}

template <>
Vector2<float>::Vector2(FXMVECTOR v) noexcept
    : Vector2Base<float>::Base{}
{
    XMStoreFloat2(this, v);
}

template<>
Vector2<int32_t>::Vector2(FXMVECTOR v) noexcept
    : Vector2Base<int32_t>::Base{} 
{
    XMStoreSInt2(this, v);
}

template<>
Vector2<uint32_t>::Vector2(FXMVECTOR v) noexcept : Vector2Base<uint32_t>::Base{}
{

}


Vector2::Vector2(const XMFLOAT2& v) noexcept
    : XMFLOAT2{v.x, v.y}
{
}

Vector2::Vector2(const XMVECTORF32& v) noexcept
    : XMFLOAT2{v.f[0], v.f[1]}
{
}

//------------------------------------------------------------------------------
// Converter operators
//------------------------------------------------------------------------------

Vector2::operator DirectX::XMVECTOR() const noexcept
{
    return XMLoadFloat2(this);
}

//------------------------------------------------------------------------------
// State manipulation
//------------------------------------------------------------------------------

/*bool Vector2::IsDirty() const noexcept
{
    return isDirty_;
}

void Vector2::ConsumeIsDirty() noexcept
{
    isDirty_ = false;
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

bool Vector2::operator==(const Vector2& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    return XMVector2Equal(v1, v2);
}

bool Vector2::operator!=(const Vector2& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

Vector2& Vector2::operator=(const XMVECTORF32& f) noexcept
{
    x = f.f[0];
    y = f.f[1];

    isDirty_ = true;

    return *this;
}

Vector2& Vector2::operator+=(const Vector2& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorAdd(v1, v2);
    XMStoreFloat2(this, x);

    isDirty_ = true;

    return *this;
}

Vector2& Vector2::operator-=(const Vector2& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorSubtract(v1, v2);
    XMStoreFloat2(this, x);

    isDirty_ = true;

    return *this;
}

Vector2& Vector2::operator*=(const Vector2& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorMultiply(v1, v2);
    XMStoreFloat2(this, x);

    isDirty_ = true;

    return *this;
}

Vector2& Vector2::operator*=(const float s) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR x = XMVectorScale(v1, s);
    XMStoreFloat2(this, x);

    isDirty_ = true;

    return *this;
}

Vector2& Vector2::operator/=(const float s) noexcept
{
    assert(s != 0.0f);

    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR X = XMVectorScale(v1, 1.f / s);
    XMStoreFloat2(this, X);

    isDirty_ = true;

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

Vector2 Vector2::operator+() const noexcept
{
    return *this;
}

Vector2 Vector2::operator-() const noexcept
{
    return Vector2(-x, -y);
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

bool Vector2::InBounds(const Vector2& bounds) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&bounds);
    return XMVector2InBounds(v1, v2);
}

float Vector2::Length() const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR x = XMVector2Length(v1);
    return XMVectorGetX(x);
}

float Vector2::LengthSquared() const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR x = XMVector2LengthSq(v1);
    return XMVectorGetX(x);
}

float Vector2::Dot(const Vector2& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVector2Dot(v1, v2);
    return XMVectorGetX(x);
}

void Vector2::Cross(const Vector2& v, Vector2& result) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR r = XMVector2Cross(v1, v2);
    XMStoreFloat2(&result, r);
}

Vector2 Vector2::Cross(const Vector2& v) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&v);
    const XMVECTOR r = XMVector2Cross(v1, v2);

    Vector2 result;
    XMStoreFloat2(&result, r);
    return result;
}

void Vector2::Normalize() noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR x = XMVector2Normalize(v1);

    isDirty_ = true;

    XMStoreFloat2(this, x);
}

void Vector2::Normalize(Vector2& result) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR x = XMVector2Normalize(v1);
    XMStoreFloat2(&result, x);
}

void Vector2::Clamp(const Vector2& vMin, const Vector2& vMax) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&vMin);
    const XMVECTOR v3 = XMLoadFloat2(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);

    isDirty_ = true;

    XMStoreFloat2(this, x);
}

void Vector2::Clamp(const Vector2& vMin, const Vector2& vMax, Vector2& result) const noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(this);
    const XMVECTOR v2 = XMLoadFloat2(&vMin);
    const XMVECTOR v3 = XMLoadFloat2(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);
    XMStoreFloat2(&result, x);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

float Vector2::Distance(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector2Length(v);
    return XMVectorGetX(x);
}

float Vector2::DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector2LengthSq(v);
    return XMVectorGetX(x);
}

void Vector2::Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Min(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Max(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Lerp(const Vector2& v1, const Vector2& v2, const float t, Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Lerp(const Vector2& v1, const Vector2& v2, const float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, const float f, const float g,
                          Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x3 = XMLoadFloat2(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, const float f,
                             const float g) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x3 = XMLoadFloat2(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, const float t,
                         Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x3 = XMLoadFloat2(&v3);
    const XMVECTOR x4 = XMLoadFloat2(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4,
                            const float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x3 = XMLoadFloat2(&v3);
    const XMVECTOR x4 = XMLoadFloat2(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, const float t,
                      Vector2& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&t1);
    const XMVECTOR x3 = XMLoadFloat2(&v2);
    const XMVECTOR x4 = XMLoadFloat2(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2,
                         const float t) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&t1);
    const XMVECTOR x3 = XMLoadFloat2(&v2);
    const XMVECTOR x4 = XMLoadFloat2(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Reflect(const Vector2& iVec, const Vector2& nVec, Vector2& result) noexcept
{
    const XMVECTOR i = XMLoadFloat2(&iVec);
    const XMVECTOR n = XMLoadFloat2(&nVec);
    const XMVECTOR x = XMVector2Reflect(i, n);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Reflect(const Vector2& iVec, const Vector2& nVec) noexcept
{
    const XMVECTOR i = XMLoadFloat2(&iVec);
    const XMVECTOR n = XMLoadFloat2(&nVec);
    const XMVECTOR x = XMVector2Reflect(i, n);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Refract(const Vector2& iVec, const Vector2& nVec, const float refractionIndex, Vector2& result) noexcept
{
    const XMVECTOR i = XMLoadFloat2(&iVec);
    const XMVECTOR n = XMLoadFloat2(&nVec);
    const XMVECTOR x = XMVector2Refract(i, n, refractionIndex);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Refract(const Vector2& iVec, const Vector2& nVec, const float refractionIndex) noexcept
{
    const XMVECTOR i = XMLoadFloat2(&iVec);
    const XMVECTOR n = XMLoadFloat2(&nVec);
    const XMVECTOR x = XMVector2Refract(i, n, refractionIndex);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    const XMVECTOR x = XMVector3Rotate(v1, q);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Transform(const Vector2& v, const Quaternion& quat) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    const XMVECTOR x = XMVector3Rotate(v1, q);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

void Vector2::Transform(const Vector2& v, const Matrix& m, Vector2& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformCoord(v1, m1);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::Transform(const Vector2& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformCoord(v1, m1);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

_Use_decl_annotations_

void Vector2::Transform(const Vector2* varray, const size_t count, const Matrix& m, Vector2* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformCoordStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, m1);
}

void Vector2::Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2Transform(v1, m1);
    XMStoreFloat4(&result, x);
}


_Use_decl_annotations_

void Vector2::Transform(const Vector2* varray, const size_t count, const Matrix& m, Vector4* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, m1);
}

void Vector2::TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformNormal(v1, m1);
    XMStoreFloat2(&result, x);
}

Vector2 Vector2::TransformNormal(const Vector2& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformNormal(v1, m1);

    Vector2 result;
    XMStoreFloat2(&result, x);
    return result;
}

_Use_decl_annotations_

void Vector2::TransformNormal(const Vector2* varray, const size_t count, const Matrix& m, Vector2* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformNormalStream(resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, m1);
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const Vector2 Vector2::Zero = {0.f, 0.f};
const Vector2 Vector2::One = {1.f, 1.f};
const Vector2 Vector2::UnitX = {1.f, 0.f};
const Vector2 Vector2::UnitY = {0.f, 1.f};*/

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------
/*
Vector2 operator+(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorAdd(x1, x2);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator-(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorSubtract(x1, x2);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator*(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorMultiply(x1, x2);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator*(const Vector2& v, const float s) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorScale(x1, s);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator/(const Vector2& v1, const Vector2& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR x2 = XMLoadFloat2(&v2);
    const XMVECTOR x = XMVectorDivide(x1, x2);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator/(const Vector2& v, const float s) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorScale(v1, 1.f / s);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}

Vector2 operator*(const float s, const Vector2& v) noexcept
{
    const XMVECTOR v1 = XMLoadFloat2(&v);
    const XMVECTOR x = XMVectorScale(v1, s);
    Vector2 r;
    XMStoreFloat2(&r, x);
    return r;
}*/

template struct Vector2<float>;
template struct Vector2<int32_t>;
template struct Vector2<uint32_t>;
