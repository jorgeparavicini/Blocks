#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Math/Vector4.h"

using namespace DirectX;
using namespace BlocksEngine;


//------------------------------------------------------------------------------
// Constructors
//------------------------------------------------------------------------------

template <class T>
Vector4<T>::Vector4() noexcept
    : Vector4Base<T>::Base(0, 0, 0, 0)
{
}

template <class T>
constexpr Vector4<T>::Vector4(const T a) noexcept
    : Vector4Base<T>::Base(a, a, a, a)
{
}

template <class T>
constexpr Vector4<T>::Vector4(const T x, const T y, const T z, const T w) noexcept
    : Vector4Base<T>::Base(x, y, z, w)
{
}

template <class T>
Vector4<T>::Vector4(_In_reads_(4) const T* pArray) noexcept
    : Vector4Base<T>::Base(pArray)
{
}

template <class T>
Vector4<T>::Vector4(FXMVECTOR v) noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    Store(this, v);
}

template <class T>
Vector4<T>::Vector4(const typename Vector4Base<T>::Base& v) noexcept
    : Vector4Base<T>::Base{v.x, v.y, v.z, v.w}
{
}

template <>
Vector4<float>::Vector4(const Vector4Base<float>::Vector& v) noexcept
    : Vector4Base<float>::Base(v.f[0], v.f[1], v.f[2], v.f[3])
{
}

template <>
Vector4<int32_t>::Vector4(const Vector4Base<int32_t>::Vector& v) noexcept
    : Vector4Base<int32_t>::Base(v.i[0], v.i[1], v.i[2], v.i[3])
{
}

template <>
Vector4<uint32_t>::Vector4(const Vector4Base<uint32_t>::Vector& v) noexcept
    : Vector4Base<uint32_t>::Base(v.u[0], v.u[1], v.u[2], v.u[3])
{
}

//------------------------------------------------------------------------------
// Convertor operators
//------------------------------------------------------------------------------

template <class T>
Vector4<T>::operator DirectX::XMVECTOR() const noexcept
{
    return Load(this);
}


//------------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------

template <class T>
bool Vector4<T>::operator==(const Vector4<T>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector4Equal(v1, v2);
}

template <class T>
bool Vector4<T>::operator!=(const Vector4<T>& v) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    return XMVector4NotEqual(v1, v2);
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

template <>
Vector4<float>& Vector4<float>::operator=(const Vector4Base<float>::Vector& v) noexcept
{
    x = v.f[0];
    y = v.f[1];
    z = v.f[2];
    w = v.f[3];

    return *this;
}

template <>
Vector4<int32_t>& Vector4<int32_t>::operator=(const Vector4Base<int32_t>::Vector& v) noexcept
{
    x = v.i[0];
    y = v.i[1];
    z = v.i[2];
    w = v.i[3];

    return *this;
}

template <>
Vector4<uint32_t>& Vector4<uint32_t>::operator=(const Vector4Base<uint32_t>::Vector& v) noexcept
{
    x = v.u[0];
    y = v.u[1];
    z = v.u[2];
    w = v.u[3];

    return *this;
}

template <class T>
template <class U>
Vector4<T>& Vector4<T>::operator+=(const Vector4<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorAdd(v1, v2);

    Store(this, x);

    return *this;
}

template <class T>
template <class U>
Vector4<T>& Vector4<T>::operator-=(const Vector4<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorSubtract(v1, v2);

    Store(this, x);

    return *this;
}

template <class T>
template <class U>
Vector4<T>& Vector4<T>::operator*=(const Vector4<U>& v) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR v2 = Load(&v);
    const XMVECTOR x = XMVectorMultiply(v1, v2);

    Store(this, x);

    return *this;
}

template <class T>
Vector4<T>& Vector4<T>::operator*=(const float s) noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorScale(v1, s);

    Store(this, x);

    return *this;
}

template <class T>
Vector4<T>& Vector4<T>::operator/=(const float s) noexcept
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
Vector4<T> Vector4<T>::operator+() const noexcept
{
    return *this;
}

template <class T>
Vector4<T> Vector4<T>::operator-() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVectorNegate(v1);

    Vector4<T> result;
    Store(&result, x);
    return result;
}


//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

template <class T>
template <class U>
bool Vector4<T>::InBounds(const Vector4<U>& v1) const noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);

    return XMVector4InBounds(x1, x2);
}

template <class T>
float Vector4<T>::Length() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector4Length(v1);
    return XMVectorGetX(x);
}

template <class T>
float Vector4<T>::LengthSquared() const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector4LengthSq(v1);
    return XMVectorGetX(x);
}

template <class T>
template <class U>
float Vector4<T>::Dot(const Vector4<U>& v1) const noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);
    const XMVECTOR x = XMVector4Dot(x1, x2);
    return XMVectorGetX(x);
}


template <class T>
template <class U, class V>
void Vector4<T>::Cross(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) const noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR r = XMVector4Cross(x1, x2, x3);

    Store(&result, r);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Cross(const Vector4<U>& v1, const Vector4<V>& v2) const noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR r = XMVector4Cross(x1, x2, x3);

    Vector4 result;
    Store(&result, r);
    return result;
}

template <class T>
void Vector4<T>::Normalize() noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector4Normalize(v1);

    Store(this, x);
}

template <class T>
void Vector4<T>::Normalize(Vector4<T>& result) const noexcept
{
    const XMVECTOR v1 = Load(this);
    const XMVECTOR x = XMVector4Normalize(v1);

    Store(&result, x);
}

template <class T>
template <class U, class V>
void Vector4<T>::Clamp(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR x = XMVectorClamp(x1, x2, x3);

    Store(this, x);
}

template <class T>
template <class U, class V>
void Vector4<T>::Clamp(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) const noexcept
{
    const XMVECTOR x1 = Load(this);
    const XMVECTOR x2 = Load(&v1);
    const XMVECTOR x3 = Load(&v2);
    const XMVECTOR x = XMVectorClamp(x1, x2, x3);

    Store(&result, x);
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

template <class T>
template <class U, class V>
float Vector4<T>::Distance(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector4Length(v);

    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
float Vector4<T>::DistanceSquared(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR v = XMVectorSubtract(x2, x1);
    const XMVECTOR x = XMVector4LengthSq(v);

    return XMVectorGetX(x);
}

template <class T>
template <class U, class V>
void Vector4<T>::Min(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Min(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMin(x1, x2);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector4<T>::Max(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Max(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMax(x1, x2);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector4<T>::Lerp(const Vector4<U>& v1, const Vector4<V>& v2, const float t, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Lerp(const Vector4<U>& v1, const Vector4<V>& v2, const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector4<T>::SmoothStep(const Vector4<U>& v1, const Vector4<V>& v2, float t, Vector4<T>& result) noexcept
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
Vector4<T> Vector4<T>::SmoothStep(const Vector4<U>& v1, const Vector4<V>& v2, float t) noexcept
{
    t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value to 0 to 1
    t = t * t * (3.f - 2.f * t);
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorLerp(x1, x2, t);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W>
void Vector4<T>::Barycentric(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const float f,
                             const float g, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W>
Vector4<T> Vector4<T>::Barycentric(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const float f,
                                   const float g) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector4<T>::CatmullRom(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                            const float t, Vector4<T>& result) noexcept
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
Vector4<T> Vector4<T>::CatmullRom(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3,
                                  const Vector4<X>& v4,
                                  const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V, class W, class X>
void Vector4<T>::Hermite(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                         const float t, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Store(&result, x);
}

template <class T>
template <class U, class V, class W, class X>
Vector4<T> Vector4<T>::Hermite(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                               const float t) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x3 = Load(&v3);
    const XMVECTOR x4 = Load(&v4);
    const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector4<T>::Reflect(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept
{
    const XMVECTOR i = Load(&v1);
    const XMVECTOR n = Load(&v2);
    const XMVECTOR x = XMVector4Reflect(i, n);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Reflect(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR i = Load(&v1);
    const XMVECTOR n = Load(&v2);
    const XMVECTOR x = XMVector4Reflect(i, n);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U, class V>
void Vector4<T>::Refract(const Vector4<U>& v1, const Vector4<V>& v2, const float refractionIndex,
                         Vector4<T>& result) noexcept
{
    const XMVECTOR i = Load(&v1);
    const XMVECTOR n = Load(&v2);
    const XMVECTOR x = XMVector4Refract(i, n, refractionIndex);

    Store(&result, x);
}

template <class T>
template <class U, class V>
Vector4<T> Vector4<T>::Refract(const Vector4<U>& v1, const Vector4<V>& v2, const float refractionIndex) noexcept
{
    const XMVECTOR i = Load(&v1);
    const XMVECTOR n = Load(&v2);
    const XMVECTOR x = XMVector4Refract(i, n, refractionIndex);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
void Vector4<T>::Transform(const Vector2<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Store(&result, x);
}

template <class T>
template <class U>
Vector4<T> Vector4<T>::Transform(const Vector2<U>& v1, const Quaternion& quat) noexcept
{
    const XMVECTOR x1 = XMLoadFloat2(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
void Vector4<T>::Transform(const Vector3<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Store(&result, x);
}

template <class T>
template <class U>
Vector4<T> Vector4<T>::Transform(const Vector3<U>& v1, const Quaternion& quat) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(g_XMIdentityR3, x, g_XMSelect1110); // result.w = 1.f

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
void Vector4<T>::Transform(const Vector4<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(x1, x, g_XMSelect1110); // result.w = v1.w

    Store(&result, x);
}

template <class T>
template <class U>
Vector4<T> Vector4<T>::Transform(const Vector4<U>& v1, const Quaternion& quat) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR q = Load(&quat);
    XMVECTOR x = XMVector3Rotate(x1, q);
    x = XMVectorSelect(x1, x, g_XMSelect1110); // result.w = v1.w

    Vector4 result;
    Store(&result, x);
    return result;
}


template <class T>
template <class U>
void Vector4<T>::Transform(const Vector2<U>& v1, const Matrix& m, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMMATRIX m1 = Load(&m);
    const XMVECTOR x = XMVector2Transform(x1, m1);

    Store(&result, x);
}


template <class T>
template <class U>
_Use_decl_annotations_

void Vector4<T>::Transform(const Vector2<U>* varray, const size_t count, const Matrix& m,
                           Vector4<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector2TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, m1);
}


template <class T>
template <class U>
void Vector4<T>::Transform(const Vector3<U>& v1, const Matrix& m, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector3Transform(x1, m1);

    Store(&result, x);
}

template <class T>
template <class U>
_Use_decl_annotations_

void Vector4<T>::Transform(const Vector3<U>* varray, const size_t count, const Matrix& m,
                           Vector4<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector3TransformStream(resultArray, sizeof XMFLOAT4, varray, sizeof XMFLOAT3, count, m1);
}

template <class T>
template <class U>
void Vector4<T>::Transform(const Vector4<U>& v1, const Matrix& m, Vector4<T>& result) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector4Transform(x1, m1);

    Store(&result, x);
}

template <class T>
template <class U>
Vector4<T> Vector4<T>::Transform(const Vector4<U>& v1, const Matrix& m) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    const XMVECTOR x = XMVector4Transform(x1, m1);

    Vector4 result;
    Store(&result, x);
    return result;
}

template <class T>
template <class U>
_Use_decl_annotations_

void Vector4<T>::Transform(const Vector4<U>* varray, const size_t count, const Matrix& m,
                           Vector4<T>* resultArray) noexcept
{
    const XMMATRIX m1 = XMLoadFloat4x4(&m);
    XMVector4TransformStream(resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, m1);
}


//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

template <class T, class U, class V>
Vector4<T> operator+(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorAdd(x1, x2);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector4<T> operator-(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorSubtract(x1, x2);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector4<T> operator*(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorMultiply(x1, x2);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U>
Vector4<T> operator*(const Vector4<U>& v, const float s) noexcept
{
    const XMVECTOR x1 = Load(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U, class V>
Vector4<T> operator/(const Vector4<U>& v1, const Vector4<V>& v2) noexcept
{
    const XMVECTOR x1 = Load(&v1);
    const XMVECTOR x2 = Load(&v2);
    const XMVECTOR x = XMVectorDivide(x1, x2);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U>
Vector4<T> operator/(const Vector4<U>& v, const float s) noexcept
{
    const XMVECTOR x1 = Load(&v);
    const XMVECTOR x = XMVectorScale(x1, 1.0f / s);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template <class T, class U>
Vector4<T> operator*(const float s, const Vector4<U>& v) noexcept
{
    const XMVECTOR x1 = Load(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    Vector2<T> r;
    Store(&r, x);
    return r;
}

template struct Vector4<float>;
template struct Vector4<int32_t>;
template struct Vector4<uint32_t>;
