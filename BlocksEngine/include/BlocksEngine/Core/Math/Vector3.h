// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vector3.h

#pragma once

#include <DirectXMath.h>
#include <iostream>
#include <PxPhysicsAPI.h>

#include "BlocksEngine/Core/Math/Math.h"

// This file is based on the DirectXTK SimpleMath header and extended for engine uses.


template <class T = float>
struct Vector3Base;

template <>
struct Vector3Base<float>
{
    using Base = DirectX::XMFLOAT3;
    using Vector = DirectX::XMVECTORF32;
};

template <>
struct Vector3Base<int32_t>
{
    using Base = DirectX::XMINT3;
    using Vector = DirectX::XMVECTORI32;
};

template <>
struct Vector3Base<uint32_t>
{
    using Base = DirectX::XMUINT3;
    using Vector = DirectX::XMVECTORU32;
};


template <typename T>
struct BlocksEngine::Vector3 : Vector3Base<T>::Base
{
    // Constructors
    Vector3() noexcept;

    constexpr Vector3(T a) noexcept;

    constexpr Vector3(const T x, const T y, const T z) noexcept
        : Vector3Base<T>::Base{x, y, z}
    {
    }

    explicit Vector3(const physx::PxVec3& vec3) noexcept;
    explicit Vector3(const physx::PxExtendedVec3& vec3) noexcept;

    explicit Vector3(_In_reads_(3) const T* pArray) noexcept;

    Vector3(DirectX::FXMVECTOR v) noexcept;

    Vector3(const typename Vector3Base<T>::Base& v) noexcept;

    explicit Vector3(const typename Vector3Base<T>::Vector& v) noexcept;

    template <class U>
    Vector3(const Vector3<U>& v) noexcept
        : Vector3Base<T>::Base{static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z)}
    {
    }

    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;

    Vector3(Vector3&&) = default;
    Vector3& operator=(Vector3&&) = default;

    ~Vector3() = default;

    //------------------------------------------------------------------------------
    // Converter operators
    //------------------------------------------------------------------------------

    operator DirectX::XMVECTOR() const noexcept;

    operator physx::PxVec3() const noexcept
    {
        return physx::PxVec3{
            static_cast<float>(this->x),
            static_cast<float>(this->y),
            static_cast<float>(this->z)
        };
    }

    /*
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
    {
        os << std::to_string(v.x) << ", " << std::to_string(v.y) << ", " << std::to_string(v.z);
        return os;
    }*/

    friend std::wostream& operator<<(std::wostream& os, const Vector3<T>& v)
    {
        os << v.x << ", " << v.y << ", " << v.z;
        return os;
    }


    // Comparison operators
    bool operator==(const Vector3<T>& v) const noexcept;
    bool operator!=(const Vector3<T>& v) const noexcept;

    // Assignment operators
    Vector3<T>& operator=(const typename Vector3Base<T>::Vector& v) noexcept;

    template <class U>
    Vector3<T>& operator+=(const Vector3<U>& v) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&v);
        const DirectX::XMVECTOR x = DirectX::XMVectorAdd(v1, v2);
        Store(this, x);

        return *this;
    }


    template <class U>
    Vector3<T>& operator-=(const Vector3<U>& v) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&v);
        const DirectX::XMVECTOR x = DirectX::XMVectorSubtract(v1, v2);
        Store(this, x);

        return *this;
    }

    template <class U>
    Vector3<T>& operator*=(const Vector3<U>& v) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&v);
        const DirectX::XMVECTOR x = DirectX::XMVectorMultiply(v1, v2);
        Store(this, x);

        return *this;
    }

    Vector3<T>& operator*=(const float s) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR x = DirectX::XMVectorScale(v1, s);
        Store(this, x);

        return *this;
    }

    Vector3<T>& operator/=(float s) noexcept;

    // Unary operators
    Vector3<T> operator+() const noexcept;
    Vector3<T> operator-() const noexcept;

    //------------------------------------------------------------------------------
    // Hashing
    //------------------------------------------------------------------------------
    friend std::size_t hash_value(const Vector3<T>& v);

    //------------------------------------------------------------------------------
    // Vector operations
    //------------------------------------------------------------------------------
    template <class U>
    [[nodiscard]] bool InBounds(const Vector3<U>& bounds) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    template <class U>
    [[nodiscard]] float Dot(const Vector3<U>& v) const noexcept;

    template <class U>
    void Cross(const Vector3<U>& v, Vector3<T>& result) const noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&v);
        const DirectX::XMVECTOR x = DirectX::XMVector3Cross(v1, v2);
        Store(&result, x);
    }

    template <class U>
    [[nodiscard]] Vector3<T> Cross(const Vector3<U>& v) const noexcept;

    void Normalize() noexcept;

    template <class U>
    void Normalize(Vector3<U>& result) const noexcept;

    template <class U, class V>
    void Clamp(const Vector3<U>& vMin, const Vector3<V>& vMax) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&vMin);
        const DirectX::XMVECTOR v3 = Load(&vMax);
        const DirectX::XMVECTOR x = DirectX::XMVectorClamp(v1, v2, v3);
        Store(this, x);
    }


    template <class U, class V>
    void Clamp(const Vector3<U>& vMin, const Vector3<V>& vMax, Vector3<T>& result) const noexcept
    {
        const DirectX::XMVECTOR v1 = Load(this);
        const DirectX::XMVECTOR v2 = Load(&vMin);
        const DirectX::XMVECTOR v3 = Load(&vMax);
        const DirectX::XMVECTOR x = DirectX::XMVectorClamp(v1, v2, v3);
        Store(&result, x);
    }

    void Abs() noexcept;

    void Abs(Vector3<T>& result) const noexcept;

    // Static functions
    template <class U, class V>
    static float Distance(const Vector3<U>& v1, const Vector3<V>& v2) noexcept;

    template <class U, class V>
    static float DistanceSquared(const Vector3<U>& v1, const Vector3<V>& v2) noexcept;


    template <class U, class V>
    static void Min(const Vector3<U>& v1, const Vector3<V>& v2, Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 Min(const Vector3<U>& v1, const Vector3<V>& v2) noexcept;


    template <class U, class V>
    static void Max(const Vector3<U>& v1, const Vector3<V>& v2, Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 Max(const Vector3<U>& v1, const Vector3<V>& v2) noexcept;


    template <class U, class V>
    static void Lerp(const Vector3<U>& v1, const Vector3<V>& v2, float t, Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 Lerp(const Vector3<U>& v1, const Vector3<V>& v2, float t) noexcept;


    template <class U, class V>
    static void SmoothStep(const Vector3<U>& v1, const Vector3<V>& v2, float t, Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 SmoothStep(const Vector3<U>& v1, const Vector3<V>& v2, float t) noexcept;


    template <class U, class V, class W>
    static void Barycentric(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, float f, float g,
                            Vector3<T>& result) noexcept;

    template <class U, class V, class W>
    static Vector3 Barycentric(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, float f,
                               float g) noexcept;


    template <class U, class V, class W, class X>
    static void CatmullRom(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, const Vector3<X>& v4,
                           float t,
                           Vector3<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector3 CatmullRom(const Vector3<U>& v1, const Vector3<V>& v2, const Vector3<W>& v3, const Vector3<X>& v4,
                              float t) noexcept;


    template <class U, class V, class W, class X>
    static void Hermite(const Vector3<U>& v1, const Vector3<V>& t1, const Vector3<W>& v2, const Vector3<X>& t2, float t,
                        Vector3<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector3 Hermite(const Vector3<U>& v1, const Vector3<V>& t1, const Vector3<W>& v2, const Vector3<X>& t2,
                           float t) noexcept;


    template <class U, class V>
    static void Reflect(const Vector3<U>& iVec, const Vector3<V>& nVec, Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 Reflect(const Vector3<U>& iVec, const Vector3<V>& nVec) noexcept;


    template <class U, class V>
    static void Refract(const Vector3<U>& iVec, const Vector3<V>& nVec, float refractionIndex,
                        Vector3<T>& result) noexcept;

    template <class U, class V>
    static Vector3 Refract(const Vector3<U>& iVec, const Vector3<V>& nVec, float refractionIndex) noexcept;


    template <class U>
    static void Transform(const Vector3<U>& v, const Quaternion& quat, Vector3<T>& result) noexcept
    {
        const DirectX::XMVECTOR x1 = Load(&v);
        const DirectX::XMVECTOR x2 = XMLoadFloat4(&quat);
        const DirectX::XMVECTOR x = DirectX::XMVector3Rotate(x1, x2);

        Store(&result, x);
    }

    template <class U>
    static Vector3 Transform(const Vector3<U>& v, const Quaternion& quat) noexcept
    {
        const DirectX::XMVECTOR x1 = Load(&v);
        const DirectX::XMVECTOR x2 = XMLoadFloat4(&quat);
        const DirectX::XMVECTOR x = DirectX::XMVector3Rotate(x1, x2);

        Vector3<T> result;
        Store(&result, x);
        return result;
    }


    template <class U>
    static void Transform(const Vector3<U>& v, const Matrix& m, Vector3<T>& result) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(&v);
        const DirectX::XMMATRIX m1 = DirectX::XMLoadFloat4x4(&m);
        const DirectX::XMVECTOR x = XMVector3TransformCoord(v1, m1);

        Store(&result, x);
    }

    template <class U>
    static Vector3 Transform(const Vector3<U>& v, const Matrix& m) noexcept
    {
        const DirectX::XMVECTOR v1 = Load(&v);
        const DirectX::XMMATRIX m1 = DirectX::XMLoadFloat4x4(&m);
        const DirectX::XMVECTOR x = XMVector3TransformCoord(v1, m1);

        Vector3<T> result;
        Store(&result, x);
        return result;
    }

    template <class U>
    static void Transform(_In_reads_(count) const Vector3<U>* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector3<T>* resultArray) noexcept;


    template <class U>
    static void TransformNormal(const Vector3<U>& v, const Matrix& m, Vector3<T>& result) noexcept;

    template <class U>
    static Vector3 TransformNormal(const Vector3<U>& v, const Matrix& m) noexcept;

    template <class U>
    static void TransformNormal(_In_reads_(count) const Vector3<U>* varray, size_t count, const Matrix& m,
                                _Out_writes_(count) Vector3<T>* resultArray) noexcept;

    // Constants
    static const Vector3<T> Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Right;
    static const Vector3 Left;
    static const Vector3 Forward;
    static const Vector3 Backward;

    //------------------------------------------------------------------------------
    // Binary operators
    //------------------------------------------------------------------------------

    template <class U>
    friend Vector3<T> operator+(const Vector3<T>& v1, const Vector3<U>& v2)
    {
        const DirectX::XMVECTOR x1 = Load(&v1);
        const DirectX::XMVECTOR x2 = v2;
        const DirectX::XMVECTOR x = DirectX::XMVectorAdd(x1, x2);

        Vector3<T> result;
        Store(&result, x);
        return result;
    }

    template <class U, class V>
    friend Vector3 operator-(const Vector3<U>& v1, const Vector3<V>& v2)
    {
        const DirectX::XMVECTOR x1 = Vector3<U>::Load(&v1);
        const DirectX::XMVECTOR x2 = Vector3<V>::Load(&v2);
        const DirectX::XMVECTOR x = DirectX::XMVectorSubtract(x1, x2);

        BlocksEngine::Vector3 result;
        Vector3<T>::Store(&result, x);
        return result;
    }

    template <class U>
    friend Vector3<T> operator*(const Vector3<T>& v1, const Vector3<U>& v2) noexcept
    {
        const DirectX::XMVECTOR x1 = Vector3<T>::Load(&v1);
        const DirectX::XMVECTOR x2 = Vector3<U>::Load(&v2);
        const DirectX::XMVECTOR x = DirectX::XMVectorMultiply(x1, x2);

        BlocksEngine::Vector3 result;
        Vector3<T>::Store(&result, x);
        return result;
    }

    friend Vector3<T> operator*(const Vector3<T>& v, const float s) noexcept
    {
        const DirectX::XMVECTOR x1 = Vector3<T>::Load(&v);
        const DirectX::XMVECTOR x = DirectX::XMVectorScale(x1, s);

        Vector3<T> result;
        Vector3<T>::Store(&result, x);
        return result;
    }

    friend Vector3<T> operator*(const Quaternion& q, const Vector3<T>& v) noexcept
    {
        const float x = q.x * 2.0f;
        const float y = q.y * 2.0f;
        const float z = q.z * 2.0f;
        const float xx = q.x * x;
        const float yy = q.y * y;
        const float zz = q.z * z;
        const float xy = q.x * y;
        const float xz = q.x * z;
        const float yz = q.y * z;
        const float wx = q.w * x;
        const float wy = q.w * y;
        const float wz = q.w * z;

        const float resX = (1.0f - (yy + zz)) * v.x + (xy - wz) * v.y + (xz + wy) * v.z;
        const float resY = (xy + wz) * v.x + (1.0f - (xx + zz)) * v.y + (yz - wx) * v.z;
        const float resZ = (xz - wy) * v.x + (yz + wx) * v.y + (1.0f - (xx + yy)) * v.z;

        Vector3<T> result;
        Vector3<T>::Store(&result, DirectX::XMVectorSet(resX, resY, resZ, 0));
        return result;
    }

    template <class U>
    friend Vector3<T> operator%(const Vector3<T>& v1, const Vector3<U>& v2) noexcept
    {
        return {v1.x % v2.x, v1.y % v2.y, v1.z % v2.z};
    }


private:
    [[nodiscard]] static DirectX::XMVECTOR Load(const typename Vector3Base<T>::Base* pSource) noexcept;
    static void Store(typename Vector3Base<T>::Base* pDestination, DirectX::FXMVECTOR v) noexcept;
};


// Constants
template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Zero{0.0f, 0.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Zero{0, 0, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Zero{0u, 0u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::One{1.0f, 1.0f, 1.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::One{1, 1, 1};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::One{1u, 1u, 1u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::UnitX{1.0f, 0.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::UnitX{1, 0, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::UnitX{1u, 0u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::UnitY{0.0f, 1.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::UnitY{0, 1, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::UnitY{0u, 1u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::UnitZ{0.0f, 0.0f, 1.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::UnitZ{0, 0, 1};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::UnitZ{0u, 0u, 1u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Up{0.0f, 1.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Up{0, 1, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Up{0u, 1u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Down{0.0f, -1.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Down{0, -1, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Down{0u, 1u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Right{1.0f, 0.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Right{1, 0, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Right{1u, 0u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Left{-1.0f, 0.0f, 0.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Left{-1, 0, 0};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Left{1u, 0u, 0u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Forward{0.0f, 0.0f, 1.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Forward{0, 0, 1};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Forward{0u, 0u, 1u};

template <>
constexpr BlocksEngine::Vector3<float> BlocksEngine::Vector3<float>::Backward{0.0f, 0.0f, -1.0f};
template <>
constexpr BlocksEngine::Vector3<int32_t> BlocksEngine::Vector3<int32_t>::Backward{0, 0, -1};
template <>
constexpr BlocksEngine::Vector3<uint32_t> BlocksEngine::Vector3<uint32_t>::Backward{0u, 0u, 1u};

//------------------------------------------------------------------------------
// Storage
//------------------------------------------------------------------------------

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector3<float>::Load(const Vector3Base<float>::Base* pSource) noexcept
{
    return XMLoadFloat3(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector3<int32_t>::Load(const Vector3Base<int32_t>::Base* pSource) noexcept
{
    return XMLoadSInt3(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector3<uint32_t>::Load(const Vector3Base<uint32_t>::Base* pSource) noexcept
{
    return XMLoadUInt3(pSource);
}

template <>
inline void BlocksEngine::Vector3<float>::Store(Vector3Base<float>::Base* pDestination, DirectX::FXMVECTOR v) noexcept
{
    XMStoreFloat3(pDestination, v);
}

template <>
inline void BlocksEngine::Vector3<int32_t>::Store(Vector3Base<int32_t>::Base* pDestination,
                                                  DirectX::FXMVECTOR v) noexcept
{
    XMStoreSInt3(pDestination, v);
}

template <>
inline void BlocksEngine::Vector3<uint32_t>::Store(Vector3Base<uint32_t>::Base* pDestination,
                                                   DirectX::FXMVECTOR v) noexcept
{
    XMStoreUInt3(pDestination, v);
}
