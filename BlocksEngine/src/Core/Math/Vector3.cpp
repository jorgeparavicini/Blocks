#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector3.h"

#include <boost/container_hash/hash.hpp>

using namespace BlocksEngine;
using namespace DirectX;

//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

template <class T>
Vector3<T>::Vector3() noexcept
    : Vector3Base<T>::Base{0, 0, 0}
{
}

template <class T>
constexpr Vector3<T>::Vector3(const T a) noexcept
    : Vector3Base<T>::Base{a, a, a}
{
}

/*template <class T>
constexpr Vector3<T>::Vector3(const T x, const T y, const T z) noexcept
    : Vector3Base<T>::Base{x, y, z}
{
}*/

template <class T>
Vector3<T>::Vector3(_In_reads_(3) const T* pArray) noexcept
    : Vector3Base<T>::Base{pArray}
{
}

template <class T>
Vector3<T>::Vector3(FXMVECTOR v) noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    Store(this, v);
}

template <class T>
Vector3<T>::Vector3(const typename Vector3Base<T>::Base& v) noexcept
    : Vector3Base<T>::Base{v.x, v.y, v.z}
{
}

template <>
Vector3<float>::Vector3(const Vector3Base<float>::Vector& v) noexcept
    : Vector3Base<float>::Base{v.f[0], v.f[1], v.f[2]}
{
}

template <>
Vector3<int32_t>::Vector3(const Vector3Base<int32_t>::Vector& v) noexcept
    : Vector3Base<int32_t>::Base{v.i[0], v.i[1], v.i[2]}
{
}

template <>
Vector3<uint32_t>::Vector3(const Vector3Base<uint32_t>::Vector& v) noexcept
    : Vector3Base<uint32_t>::Base{v.u[0], v.u[1], v.u[2]}
{
}

//------------------------------------------------------------------------------
// Converter operators
//------------------------------------------------------------------------------

template <class T>
Vector3<T>::operator XMVECTOR() const noexcept
{
    return Load(this);
}

//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

template <class T>
bool Vector3<T>::operator==(const Vector3& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector3Equal(v1, v2);
}

template <class T>
bool Vector3<T>::operator!=(const Vector3& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector3NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

template <>
Vector3<float>& Vector3<float>::operator=(const Vector3Base<float>::Vector& v) noexcept
{
    this->x = v.f[0];
    this->y = v.f[1];
    this->z = v.f[2];

    return *this;
}

template <>
Vector3<int32_t>& Vector3<int32_t>::operator=(const Vector3Base<int32_t>::Vector& v) noexcept
{
    this->x = v.i[0];
    this->y = v.i[1];
    this->z = v.i[2];

    return *this;
}

template <>
Vector3<uint32_t>& Vector3<uint32_t>::operator=(const Vector3Base<uint32_t>::Vector& v) noexcept
{
    this->x = v.u[0];
    this->y = v.u[1];
    this->z = v.u[2];

    return *this;
}

template <class T>
Vector3<T>& Vector3<T>::operator/=(const float s) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorScale(v1, 1.0f / s);
    Store(this, x);

    return *this;
}

//------------------------------------------------------------------------------
// Unary operators
//------------------------------------------------------------------------------

template <class T>
Vector3<T> Vector3<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
Vector3<T> Vector3<T>::operator-() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorNegate(v1);

    Vector3<T> r;
    Store(&r, x);
    return r;
}

//------------------------------------------------------------------------------
// Hashing
//------------------------------------------------------------------------------

template <class T>
std::size_t hash_value(const Vector3<T>& v)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, v.x);
    boost::hash_combine(seed, v.y);
    boost::hash_combine(seed, v.z);

    return seed;
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

template <class T>
template <class U>
bool Vector3<T>::InBounds(const Vector3<U>& bounds) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&bounds);
    return XMVector3InBounds(v1, v2);
}

template <class T>
float Vector3<T>::Length() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector3Length(v1);
    return XMVectorGetX(x);
}

template <class T>
float Vector3<T>::LengthSquared() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector3LengthSq(v1);
    return XMVectorGetX(x);
}

template <class T>
template <class U>
float Vector3<T>::Dot(const Vector3<U>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVector3Dot(v1, v2);
    return XMVectorGetX(x);
}

template <class T>
template <class U>
void Vector3<T>::Cross(const Vector3<U>& v, Vector3<T>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVector3Cross(v1, v2);
    Store(&result, x);
}

template <class T>
template <class U>
Vector3<T> Vector3<T>::Cross(const Vector3<U>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVector3Cross(v1, v2);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
void Vector3<T>::Normalize() noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector3Normalize(v1);
    Store(this, x);
}

template <class T>
template <class U>
void Vector3<T>::Normalize(Vector3<U>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector3Normalize(v1);
    Store(&result, x);
}


//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

template <typename T>
void Vector3<T>::Abs() noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorAbs(v1);
    Store(this, x);
}

template <typename T>
void Vector3<T>::Abs(Vector3<T>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorAbs(v1);
    Store(&result, x);
}

template <class T>
template <class U, class V>
float Vector3<T>::Distance(const Vector3<U>& v1, const Vector3<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector3Length(v);

    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
float Vector3<T>::DistanceSquared(const Vector3<U>& v1, const Vector3<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector3LengthSq(v);
    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
void Vector3<T>::Min(const Vector3<U>& v1, const Vector3<V>& v2, Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::Min(const Vector3<U>& v1, const Vector3<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x2, x1);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector3<T>::Max(const Vector3<U>& v1, const Vector3<V>& v2, Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x2, x1);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::Max(const Vector3<U>& v1, const Vector3<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x2, x1);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector3<T>::Lerp(const Vector3<U>& v1, const Vector3<V>& v2, const float t, Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x2, x1, t);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::Lerp(const Vector3<U>& v1, const Vector3<V>& v2, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x2, x1, t);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector3<T>::SmoothStep(const Vector3<U>& v1, const Vector3<V>& v2, float t, Vector3<T>& result) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value between 0 and 1
    t = t * t * (3.0f - 2.0f * t);
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::SmoothStep(const Vector3<U>& v1, const Vector3<V>& v2, float t) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value between 0 and 1
    t = t * t * (3.0f - 2.0f * t);
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W>
void Vector3<T>::Barycentric(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, const float f,
                             const float g, Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W>
Vector3<T> Vector3<T>::Barycentric(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3,
                                   const float f, const float g) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector3<T>::CatmullRom(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, const Vector3<X>& v4,
                            const float t, Vector3<T>& result) noexcept
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
Vector3<T> Vector3<T>::CatmullRom(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3,
                                  const Vector3<X>& v4, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector3<T>::Hermite(const Vector3<U>& v1, const Vector3<V>& t1, const Vector3<W>& v2, const Vector3<X>& t2,
                         const float t, Vector3<T>& result) noexcept
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
Vector3<T> Vector3<T>::Hermite(const Vector3<U>& v1, const Vector3<V>& t1, const Vector3<W>& v2,
                               const Vector3<X>& t2, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&t1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR x4 = Load(&t2);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector3<T>::Reflect(const Vector3<U>& iVec, const Vector3<V>& nVec, Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&iVec);
    const XMVECTOR x2 = Load(&nVec);
    const XMVECTOR x = XMVector3Reflect(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::Reflect(const Vector3<U>& iVec, const Vector3<V>& nVec) noexcept
{
    const XMVECTOR x1 = Load(&iVec);
    const XMVECTOR x2 = Load(&nVec);
    const XMVECTOR x = XMVector3Reflect(x1, x2);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector3<T>::Refract(const Vector3<U>& iVec, const Vector3<V>& nVec, const float refractionIndex,
                         Vector3<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&iVec);
    const XMVECTOR x2 = Load(&nVec);
    const XMVECTOR x = XMVector3Refract(x1, x2, refractionIndex);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector3<T> Vector3<T>::Refract(const Vector3<U>& iVec, const Vector3<V>& nVec,
                               const float refractionIndex) noexcept
{
    const XMVECTOR x1 = Load(&iVec);
    const XMVECTOR x2 = Load(&nVec);
    const XMVECTOR x = XMVector3Refract(x1, x2, refractionIndex);

    Vector3<T> result;
    Store(&result, x);
    return result;
}


template <class T>
template <class U>
_Use_decl_annotations_

void Vector3<T>::Transform(const Vector3<U>* varray, const size_t count, const Matrix& m,
                           Vector3<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector3TransformCoordStream(resultArray, sizeof Vector3Base<U>::Vector, varray, sizeof Vector3Base<T>::Vector,
                                  count, m1);
}

template <class T>
template <class U>
void Vector3<T>::TransformNormal(const Vector3<U>& v, const Matrix& m, Vector3<T>& result) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector3TransformNormal(v1, m1);

    Store(&result, x);
}

template <class T>
template <class U>
Vector3<T> Vector3<T>::TransformNormal(const Vector3<U>& v, const Matrix& m) noexcept
{
    const XMVECTOR v1 = Load(&v);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector3TransformNormal(v1, m1);

    Vector3<T> result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
_Use_decl_annotations_

void Vector3<T>::TransformNormal(const Vector3<U>* varray, const size_t count, const Matrix& m,
                                 Vector3<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector3TransformNormalStream(resultArray, sizeof Vector3Base<U>::Vector, varray, sizeof Vector3Base<T>::Vector,
                                   count, m1);
}

template struct Vector3<float>;
template struct Vector3<int32_t>;
template struct Vector3<uint32_t>;
