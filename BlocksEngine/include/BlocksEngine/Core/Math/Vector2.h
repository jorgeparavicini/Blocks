// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vector2.h

#pragma once

#include <DirectXMath.h>
#include <boost/container_hash/hash.hpp>

#include "BlocksEngine/Core/Math/Math.h"

// This file is based on the DirectXTK SimpleMath header and extended for engine uses.


template <class T = float>
struct Vector2Base;

template <>
struct Vector2Base<float>
{
    using Base = DirectX::XMFLOAT2;
    using Vector = DirectX::XMVECTORF32;
};

template <>
struct Vector2Base<int32_t>
{
    using Base = DirectX::XMINT2;
    using Vector = DirectX::XMVECTORI32;
};

template <>
struct Vector2Base<uint32_t>
{
    using Base = DirectX::XMUINT2;
    using Vector = DirectX::XMVECTORU32;
};

template <class T>
struct BlocksEngine::Vector2 : Vector2Base<T>::Base
{
    // Constructors
    Vector2() noexcept;

    constexpr explicit Vector2(T a) noexcept;

    constexpr Vector2<T>(T x, T y) noexcept
        : Vector2Base<T>::Base{x, y}
    {
    }


    explicit Vector2(_In_reads_(2) const T* pArray) noexcept;

    Vector2(DirectX::FXMVECTOR v) noexcept;

    Vector2(const typename Vector2Base<T>::Base& v) noexcept;

    explicit Vector2(const typename Vector2Base<T>::Vector& v) noexcept;

    Vector2(const Vector2&) = default;
    Vector2& operator=(const Vector2&) = default;

    Vector2(Vector2&&) = default;
    Vector2& operator=(Vector2&&) = default;

    ~Vector2() = default;

    // Converter operators

    operator DirectX::XMVECTOR() const noexcept;

    // Comparison operators
    bool operator ==(const Vector2<T>& v) const noexcept;
    bool operator !=(const Vector2<T>& v) const noexcept;

    // Assignment operators
    // TODO: This does not allow assigning other types currently, but could be implemented. But which type should be returned?
    Vector2<T>& operator=(const typename Vector2Base<T>::Vector& f) noexcept;

    template <class U>
    Vector2<T>& operator+=(const Vector2<U>& v) noexcept;

    template <class U>
    Vector2<T>& operator-=(const Vector2<U>& v) noexcept;

    template <class U>
    Vector2<T>& operator*=(const Vector2<U>& v) noexcept;
    Vector2<T>& operator*=(float s) noexcept;
    Vector2<T>& operator/=(float s) noexcept;

    // Unary operators
    Vector2<T> operator+() const noexcept;
    Vector2<T> operator-() const noexcept;

    //------------------------------------------------------------------------------
    // Hashing
    //------------------------------------------------------------------------------
    friend std::size_t hash_value(const Vector2<T>& v)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, v.x);
        boost::hash_combine(seed, v.y);

        return seed;
    }

    // Vector operations
    template <class U>
    [[nodiscard]] bool InBounds(const Vector2<U>& bounds) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    template <class U>
    [[nodiscard]] float Dot(const Vector2<U>& v) const noexcept;

    template <class U, class V>
    void Cross(const Vector2<U>& v, Vector2<V>& result) const noexcept;

    template <class U, class V>
    [[nodiscard]] Vector2<U> Cross(const Vector2<V>& v) const noexcept;

    void Normalize() noexcept;

    template <class U>
    void Normalize(Vector2<U>& result) const noexcept;

    template <class U, class V>
    void Clamp(const Vector2<U>& vMin, const Vector2<V>& vMax) noexcept;

    template <class U, class V, class W>
    void Clamp(const Vector2<U>& vMin, const Vector2<V>& vMax, Vector2<W>& result) const noexcept;

    // Static functions
    template <class U, class V>
    static float Distance(const Vector2<U>& v1, const Vector2<V>& v2) noexcept;

    template <class U, class V>
    static float DistanceSquared(const Vector2<U>& v1, const Vector2<V>& v2) noexcept;


    template <class U, class V>
    static void Min(const Vector2<U>& v1, const Vector2<V>& v2, Vector2<T>& result) noexcept;
    template <class U, class V>
    static Vector2<T> Min(const Vector2<U>& v1, const Vector2<V>& v2) noexcept;


    template <class U, class V>
    static void Max(const Vector2<U>& v1, const Vector2<V>& v2, Vector2<T>& result) noexcept;

    template <class U, class V>
    static Vector2<T> Max(const Vector2<U>& v1, const Vector2<V>& v2) noexcept;


    template <class U, class V>
    static void Lerp(const Vector2<U>& v1, const Vector2<V>& v2, float t, Vector2<T>& result) noexcept;

    template <class U, class V>
    static Vector2<T> Lerp(const Vector2<U>& v1, const Vector2<V>& v2, float t) noexcept;


    template <class U, class V>
    static void SmoothStep(const Vector2<U>& v1, const Vector2<V>& v2, float t, Vector2<T>& result) noexcept;

    template <class U, class V>
    static Vector2<T> SmoothStep(const Vector2<U>& v1, const Vector2<V>& v2, float t) noexcept;


    template <class U, class V, class W>
    static void Barycentric(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, float f, float g,
                            Vector2<T>& result) noexcept;

    template <class U, class V, class W>
    static Vector2<T> Barycentric(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, float f,
                                  float g) noexcept;


    template <class U, class V, class W, class X>
    static void CatmullRom(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, const Vector2<X>& v4,
                           float t, Vector2<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector2<T> CatmullRom(const Vector2<U>& v1, const Vector2<V>& v2, const Vector2<W>& v3, const Vector2<X>& v4,
                                 float t) noexcept;


    template <class U, class V, class W, class X>
    static void Hermite(const Vector2<U>& v1, const Vector2<V>& t1, const Vector2<W>& v2, const Vector2<X>& t2, float t,
                        Vector2<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector2<T> Hermite(const Vector2<U>& v1, const Vector2<V>& t1, const Vector2<W>& v2, const Vector2<X>& t2,
                              float t) noexcept;


    template <class U, class V>
    static void Reflect(const Vector2<U>& iVec, const Vector2<V>& nVec, Vector2<T>& result) noexcept;

    template <class U, class V>
    static Vector2<T> Reflect(const Vector2<U>& iVec, const Vector2<V>& nVec) noexcept;

    template <class U, class V>
    static void Refract(const Vector2<U>& iVec, const Vector2<V>& nVec, float refractionIndex,
                        Vector2<T>& result) noexcept;

    template <class U, class V>
    static Vector2<T> Refract(const Vector2<U>& iVec, const Vector2<V>& nVec, float refractionIndex) noexcept;


    template <class U>
    static void Transform(const Vector2<U>& v, const Quaternion& quat, Vector2<T>& result) noexcept;

    template <class U>
    static Vector2<T> Transform(const Vector2<U>& v, const Quaternion& quat) noexcept;

    template <class U>
    static void Transform(const Vector2<U>& v, const Matrix& m, Vector2<T>& result) noexcept;

    template <class U>
    static Vector2<T> Transform(const Vector2<U>& v, const Matrix& m) noexcept;

    template <class U>
    static void Transform(_In_reads_(count) const Vector2<U>* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector2<T>* resultArray) noexcept;


    template <class U>
    static void TransformNormal(const Vector2<U>& v, const Matrix& m, Vector2<T>& result) noexcept;

    template <class U>
    static Vector2<T> TransformNormal(const Vector2<U>& v, const Matrix& m) noexcept;

    template <class U>
    static void TransformNormal(_In_reads_(count) const Vector2<U>* varray, size_t count, const Matrix& m,
                                _Out_writes_(count) Vector2<T>* resultArray) noexcept;

    // Constants
    static const Vector2<T> Zero;
    static const Vector2<T> One;
    static const Vector2<T> UnitX;
    static const Vector2<T> UnitY;

private:
    [[nodiscard]] static DirectX::XMVECTOR Load(const typename Vector2Base<T>::Base* pSource) noexcept;
    static void Store(typename Vector2Base<T>::Base* pDestination, DirectX::FXMVECTOR v) noexcept;
};

// Binary operators
template <class T, class U, class V>
BlocksEngine::Vector2<T> operator+(const BlocksEngine::Vector2<U>& v1, const BlocksEngine::Vector2<V>& v2) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector2<T> operator-(const BlocksEngine::Vector2<U>& v1, const BlocksEngine::Vector2<V>& v2) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector2<T> operator*(const BlocksEngine::Vector2<U>& v1, const BlocksEngine::Vector2<V>& v2) noexcept;

template <class T, class U>
BlocksEngine::Vector2<T> operator*(const BlocksEngine::Vector2<U>& v, float s) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector2<T> operator/(const BlocksEngine::Vector2<U>& v1, const BlocksEngine::Vector2<V>& v2) noexcept;

template <class T, class U>
BlocksEngine::Vector2<T> operator/(const BlocksEngine::Vector2<U>& v, float s) noexcept;

template <class T, class U>
BlocksEngine::Vector2<T> operator*(float s, const BlocksEngine::Vector2<U>& v) noexcept;


// Constants
template <>
const BlocksEngine::Vector2<float> BlocksEngine::Vector2<float>::Zero{0.0f, 0.0f};
template <>
const BlocksEngine::Vector2<int32_t> BlocksEngine::Vector2<int32_t>::Zero{0, 0};
template <>
const BlocksEngine::Vector2<uint32_t> BlocksEngine::Vector2<uint32_t>::Zero{0u, 0u};

template <>
const BlocksEngine::Vector2<float> BlocksEngine::Vector2<float>::One{1.0f, 1.0f};
template <>
const BlocksEngine::Vector2<int32_t> BlocksEngine::Vector2<int32_t>::One{1, 1};
template <>
const BlocksEngine::Vector2<uint32_t> BlocksEngine::Vector2<uint32_t>::One{1u, 1u};

template <>
const BlocksEngine::Vector2<float> BlocksEngine::Vector2<float>::UnitX{1.0f, 0.0f};
template <>
const BlocksEngine::Vector2<int32_t> BlocksEngine::Vector2<int32_t>::UnitX{1, 0};
template <>
const BlocksEngine::Vector2<uint32_t> BlocksEngine::Vector2<uint32_t>::UnitX{1u, 0u};

template <>
const BlocksEngine::Vector2<float> BlocksEngine::Vector2<float>::UnitY{0.0f, 1.0f};
template <>
const BlocksEngine::Vector2<int32_t> BlocksEngine::Vector2<int32_t>::UnitY{0, 1};
template <>
const BlocksEngine::Vector2<uint32_t> BlocksEngine::Vector2<uint32_t>::UnitY{0u, 1u};

//------------------------------------------------------------------------------
// Storage
//------------------------------------------------------------------------------

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector2<float>::Load(const Vector2Base<float>::Base* pSource) noexcept
{
    return XMLoadFloat2(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector2<int32_t>::Load(const Vector2Base<int32_t>::Base* pSource) noexcept
{
    return XMLoadSInt2(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector2<uint32_t>::Load(const Vector2Base<uint32_t>::Base* pSource) noexcept
{
    return XMLoadUInt2(pSource);
}

template <>
inline void BlocksEngine::Vector2<float>::Store(Vector2Base<float>::Base* pDestination, DirectX::FXMVECTOR v) noexcept
{
    XMStoreFloat2(pDestination, v);
}

template <>
inline void BlocksEngine::Vector2<int32_t>::Store(Vector2Base<int32_t>::Base* pDestination,
                                                  DirectX::FXMVECTOR v) noexcept
{
    XMStoreSInt2(pDestination, v);
}

template <>
inline void BlocksEngine::Vector2<uint32_t>::Store(Vector2Base<uint32_t>::Base* pDestination,
                                                   DirectX::FXMVECTOR v) noexcept
{
    XMStoreUInt2(pDestination, v);
}
