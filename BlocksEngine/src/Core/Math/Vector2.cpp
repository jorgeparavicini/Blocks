#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector2.h"

#include <boost/container_hash/hash.hpp>

#include "BlocksEngine/Quaternion.h"

using namespace BlocksEngine;
using namespace DirectX;

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
Vector2<T>::Vector2(_In_reads_(2) const T* pArray) noexcept
    : Vector2Base<T>::Base{pArray}
{
}

template <class T>
Vector2<T>::Vector2(FXMVECTOR v) noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    Store(this, v);
}

template <class T>
Vector2<T>::Vector2(const typename Vector2Base<T>::Base& v) noexcept
    : Vector2Base<T>::Base{v.x, v.y}
{
}

template <>
Vector2<float>::Vector2(const Vector2Base<float>::Vector& v) noexcept
    : Vector2Base<float>::Base{v.f[0], v.f[1]}
{
}

template <>
Vector2<int32_t>::Vector2(const Vector2Base<int32_t>::Vector& v) noexcept
    : Vector2Base<int32_t>::Base{v.i[0], v.i[1]}
{
}

template <>
Vector2<uint32_t>::Vector2(const Vector2Base<uint32_t>::Vector& v) noexcept
    : Vector2Base<uint32_t>::Base{v.u[0], v.u[1]}
{
}

//------------------------------------------------------------------------------
// Converter operators
//------------------------------------------------------------------------------

template <class T>
Vector2<T>::operator DirectX::XMVECTOR() const noexcept
{
    return Load(this);
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

template <class T>
bool Vector2<T>::operator==(const Vector2<T>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector2Equal(v1, v2);
}

template <class T>
bool Vector2<T>::operator!=(const Vector2<T>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector2NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

template <>
Vector2<float>& Vector2<float>::operator=(const Vector2Base<float>::Vector& f) noexcept
{
    this->x = f.f[0];
    this->y = f.f[1];

    return *this;
}

template <>
Vector2<int32_t>& Vector2<int32_t>::operator=(const Vector2Base<int32_t>::Vector& f) noexcept
{
    this->x = f.i[0];
    this->y = f.i[1];

    return *this;
}

template <>
Vector2<uint32_t>& Vector2<uint32_t>::operator=(const Vector2Base<uint32_t>::Vector& f) noexcept
{
    this->x = f.u[0];
    this->y = f.u[1];

    return *this;
}

template <class T>
template <class U>
Vector2<T>& Vector2<T>::operator+=(const Vector2<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorAdd(v1, v2);
    Store(this, x);

    return *this;
}

template <class T>
template <class U>
Vector2<T>& Vector2<T>::operator-=(const Vector2<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorSubtract(v1, v2);
    Store(this, x);

    return *this;
}

template <class T>
template <class U>
Vector2<T>& Vector2<T>::operator*=(const Vector2<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorMultiply(v1, v2);
    Store(this, x);

    return *this;
}

template <class T>
Vector2<T>& Vector2<T>::operator*=(const float s) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorScale(v1, s);
    Store(this, x);

    return *this;
}

template <class T>
Vector2<T>& Vector2<T>::operator/=(const float s) noexcept
{
    assert(s != 0.0f);

    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorScale(v1, 1.f / s);
    Store(this, x);

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

template <class T>
Vector2<T> Vector2<T>::operator+() const noexcept
{
    return *this;
}

// TODO: Could be done with XMVectorNegate, but test it
template <>
Vector2<float> Vector2<float>::operator-() const noexcept
{
    return Vector2(-this->x, -this->y);
}

template <>
Vector2<int32_t> Vector2<int32_t>::operator-() const noexcept
{
    return Vector2(-this->x, -this->y);
}

template <>
Vector2<uint32_t> Vector2<uint32_t>::operator-() const noexcept
{
    return Vector2(this->x, this->y);
}

//------------------------------------------------------------------------------
// Hashing
//------------------------------------------------------------------------------

/*template <class T>
std::size_t hash_value(const Vector2<T>& v)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, v.x);
    boost::hash_combine(seed, v.y);

    return seed;
}*/

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

template <class T>
template <class U>
bool Vector2<T>::InBounds(const Vector2<U>& bounds) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&bounds);
    return XMVector2InBounds(v1, v2);
}

template <class T>
float Vector2<T>::Length() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector2Length(v1);
    return XMVectorGetX(x);
}

template <class T>
float Vector2<T>::LengthSquared() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector2LengthSq(v1);
    return XMVectorGetX(x);
}

template <class T>
template <class U>
float Vector2<T>::Dot(const Vector2<U>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVector2Dot(v1, v2);
    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
void Vector2<T>::Cross(const Vector2<U>& v, Vector2<V>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR r = XMVector2Cross(v1, v2);
    Store(&result, r);
}

template <class T>
template <class U, class V>
Vector2<U> Vector2<T>::Cross(const Vector2<V>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR r = XMVector2Cross(v1, v2);

    Vector2<U> result;
    Store(&result, r);
    return result;
}

template <class T>
void Vector2<T>::Normalize() noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector2Normalize(v1);

    Store(this, x);
}

template <class T>
template <class U>
void Vector2<T>::Normalize(Vector2<U>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector2Normalize(v1);
    Store(&result, x);
}

template <class T>
template <class U, class V>
void Vector2<T>::Clamp(const Vector2<U>& vMin, const Vector2<V>& vMax) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&vMin);
    const XMVECTOR v3 = Load(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);

    Store(this, x);
}

template <class T>
template <class U, class V, class W>
void Vector2<T>::Clamp(const Vector2<U>& vMin, const Vector2<V>& vMax, Vector2<W>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&vMin);
    const XMVECTOR v3 = Load(&vMax);
    const XMVECTOR x = XMVectorClamp(v1, v2, v3);

    Store(&result, x);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

template <class T>
template <class U, class V>
float Vector2<T>::Distance(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector2Length(v);

    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
float Vector2<T>::DistanceSquared(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector2LengthSq(v);

    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
void Vector2<T>::Min(const Vector2<U>& v1, const Vector2<V>& v2, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::Min(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector2<T>::Max(const Vector2<U>& v1, const Vector2<V>& v2, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::Max(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector2<T>::Lerp(const Vector2<U>& v1, const Vector2<V>& v2, const float t, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::Lerp(const Vector2<U>& v1, const Vector2<V>& v2, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector2<T>::SmoothStep(const Vector2<U>& v1, const Vector2<V>& v2, float t, Vector2<T>& result) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::SmoothStep(const Vector2<U>& v1, const Vector2<V>& v2, float t) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W>
void Vector2<T>::Barycentric(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, const float f,
                             const float g, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W>
Vector2<T> Vector2<T>::Barycentric(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, const float f,
                                   const float g) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector2<T>::CatmullRom(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, const Vector2<X>& v4,
                            const float t, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W, class X>
Vector2<T> Vector2<T>::CatmullRom(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3,
                                  const Vector2<X>& v4, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector2<T>::Hermite(const Vector2<U>& v1, const Vector2<V>& t1, const Vector2<W>& v2, const Vector2<X>& t2,
                         const float t, Vector2<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&t1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR x4 = Load(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W, class X>
Vector2<T> Vector2<T>::Hermite(const Vector2<U>& v1, const Vector2<V>& t1, const Vector2<W>& v2, const Vector2<X>& t2,
                               const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&t1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR x4 = Load(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector2<T>::Reflect(const Vector2<U>& iVec, const Vector2<V>& nVec, Vector2<T>& result) noexcept
{
    const XMVECTOR i = Load(&iVec);
    const XMVECTOR n = Load(&nVec);
    const XMVECTOR x = XMVector2Reflect(i, n);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::Reflect(const Vector2<U>& iVec, const Vector2<V>& nVec) noexcept
{
    const XMVECTOR i = Load(&iVec);
    const XMVECTOR n = Load(&nVec);
    const XMVECTOR x = XMVector2Reflect(i, n);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector2<T>::Refract(const Vector2<U>& iVec, const Vector2<V>& nVec, const float refractionIndex,
                         Vector2<T>& result) noexcept
{
    const XMVECTOR i = Load(&iVec);
    const XMVECTOR n = Load(&nVec);
    const XMVECTOR x = XMVector2Refract(i, n, refractionIndex);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector2<T> Vector2<T>::Refract(const Vector2<U>& iVec, const Vector2<V>& nVec, const float refractionIndex) noexcept
{
    const XMVECTOR i = Load(&iVec);
    const XMVECTOR n = Load(&nVec);
    const XMVECTOR x = XMVector2Refract(i, n, refractionIndex);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
void Vector2<T>::Transform(const Vector2<U>& v, const Quaternion& quat, Vector2<T>& result) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    const XMVECTOR x = XMVector3Rotate(v1, q);

    Store(&result, x);
}

template <class T>
template <class U>
Vector2<T> Vector2<T>::Transform(const Vector2<U>& v, const Quaternion& quat) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMVECTOR q = XMLoadFloat4(&quat);
    const XMVECTOR x = XMVector3Rotate(v1, q);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
void Vector2<T>::Transform(const Vector2<U>& v, const Matrix& m, Vector2<T>& result) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformCoord(v1, m1);

    Store(&result, x);
}

template <class T>
template <class U>
Vector2<T> Vector2<T>::Transform(const Vector2<U>& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformCoord(v1, m1);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
_Use_decl_annotations_

void Vector2<T>::Transform(const Vector2<U>* varray, const size_t count, const Matrix& m,
                           Vector2<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformCoordStream(resultArray, sizeof Vector2Base<U>::Vector, varray, sizeof Vector2Base<T>::Vector,
                                  count, m1);
}


template <class T>
template <class U>
void Vector2<T>::TransformNormal(const Vector2<U>& v, const Matrix& m, Vector2<T>& result) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformNormal(v1, m1);

    Store(&result, x);
}

template <class T>
template <class U>
Vector2<T> Vector2<T>::TransformNormal(const Vector2<U>& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector2TransformNormal(v1, m1);

    Vector2<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
_Use_decl_annotations_

void Vector2<T>::TransformNormal(const Vector2<U>* varray, const size_t count, const Matrix& m,
                                 Vector2<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformNormalStream(resultArray, sizeof Vector2Base<U>::Vector, varray, sizeof Vector2Base<T>::Vector,
                                   count, m1);
}


//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

template <class T, class U, class V>
Vector2<T> operator+(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorAdd(x1, x2);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector2<T> operator-(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorSubtract(x1, x2);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector2<T> operator*(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMultiply(x1, x2);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U>
Vector2<T> operator*(const Vector2<U>& v, const float s) noexcept
{
    const XMVECTOR x1 = Load(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector2<T> operator/(const Vector2<U>& v1, const Vector2<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorDivide(x1, x2);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U>
Vector2<T> operator/(const Vector2<U>& v, const float s) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMVECTOR x = XMVectorScale(v1, 1.f / s);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template <class T, class U>
Vector2<T> operator*(const float s, const Vector2<U>& v) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMVECTOR x = XMVectorScale(v1, s);

    Vector2<T> r;
    Vector2<T>::Store(&r, x);
    return r;
}

template struct Vector2<float>;
template struct Vector2<int32_t>;
template struct Vector2<uint32_t>;
