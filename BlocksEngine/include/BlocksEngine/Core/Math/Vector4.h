// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vector4.h

#pragma once

#include <DirectXMath.h>

#include "BlocksEngine/Core/Math/Math.h"

// This file is based on the DirectXTK SimpleMath header and extended for engine uses.

namespace BlocksEngine
{
    template <class T>
    struct Vector4;
}

template <class T = float>
struct Vector4Base;

template <>
struct Vector4Base<float>
{
    using Base = DirectX::XMFLOAT4;
    using Vector = DirectX::XMVECTORF32;
};

template <>
struct Vector4Base<int32_t>
{
    using Base = DirectX::XMINT4;
    using Vector = DirectX::XMVECTORI32;
};

template <>
struct Vector4Base<uint32_t>
{
    using Base = DirectX::XMUINT4;
    using Vector = DirectX::XMVECTORU32;
};


template <class T>
struct BlocksEngine::Vector4 : Vector4Base<T>::Base
{
    // Constructors
    Vector4() noexcept;

    constexpr explicit Vector4(T a) noexcept;

    constexpr Vector4(T x, T y, T z, T w) noexcept;

    explicit Vector4(_In_reads_(4) const T* pArray) noexcept;

    Vector4(DirectX::FXMVECTOR v) noexcept;

    Vector4(const typename Vector4Base<T>::Base& v) noexcept;

    explicit Vector4(const typename Vector4Base<T>::Vector& v) noexcept;

    Vector4(const Vector4&) = default;
    Vector4& operator=(const Vector4&) = default;

    Vector4(Vector4&&) = default;
    Vector4& operator=(Vector4&&) = default;

    ~Vector4() = default;

    // Converter operators

    operator DirectX::XMVECTOR() const noexcept;

    // Comparison operators
    bool operator ==(const Vector4<T>& v) const noexcept;
    bool operator !=(const Vector4<T>& v) const noexcept;

    // Assignment operators
    Vector4<T>& operator=(const typename Vector4Base<T>::Vector& v) noexcept;

    template <class U>
    Vector4<T>& operator+=(const Vector4<U>& v) noexcept;

    template <class U>
    Vector4<T>& operator-=(const Vector4<U>& v) noexcept;

    template <class U>
    Vector4<T>& operator*=(const Vector4<U>& v) noexcept;
    Vector4<T>& operator*=(float s) noexcept;
    Vector4<T>& operator/=(float s) noexcept;

    // Unary operators
    Vector4<T> operator+() const noexcept;
    Vector4<T> operator-() const noexcept;

    // Vector operations
    template <class U>
    [[nodiscard]] bool InBounds(const Vector4<U>& v1) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    template <class U>
    [[nodiscard]] float Dot(const Vector4<U>& v1) const noexcept;

    template <class U, class V>
    void Cross(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) const noexcept;

    template <class U, class V>
    [[nodiscard]] Vector4 Cross(const Vector4<U>& v1, const Vector4<V>& v2) const noexcept;

    void Normalize() noexcept;
    void Normalize(Vector4<T>& result) const noexcept;


    template <class U, class V>
    void Clamp(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;

    template <class U, class V>
    void Clamp(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) const noexcept;

    // Static functions

    template <class U, class V>
    static float Distance(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;

    template <class U, class V>
    static float DistanceSquared(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;


    template <class U, class V>
    static void Min(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 Min(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;


    template <class U, class V>
    static void Max(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 Max(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;


    template <class U, class V>
    static void Lerp(const Vector4<U>& v1, const Vector4<V>& v2, float t, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 Lerp(const Vector4<U>& v1, const Vector4<V>& v2, float t) noexcept;


    template <class U, class V>
    static void SmoothStep(const Vector4<U>& v1, const Vector4<V>& v2, float t, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 SmoothStep(const Vector4<U>& v1, const Vector4<V>& v2, float t) noexcept;


    template <class U, class V, class W>
    static void Barycentric(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, float f, float g,
                            Vector4<T>& result) noexcept;


    template <class U, class V, class W>
    static Vector4 Barycentric(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, float f,
                               float g) noexcept;


    template <class U, class V, class W, class X>
    static void CatmullRom(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                           float t,
                           Vector4<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector4 CatmullRom(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                              float t) noexcept;


    template <class U, class V, class W, class X>
    static void Hermite(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4, float t,
                        Vector4<T>& result) noexcept;

    template <class U, class V, class W, class X>
    static Vector4 Hermite(const Vector4<U>& v1, const Vector4<V>& v2, const Vector4<W>& v3, const Vector4<X>& v4,
                           float t) noexcept;


    template <class U, class V>
    static void Reflect(const Vector4<U>& v1, const Vector4<V>& v2, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 Reflect(const Vector4<U>& v1, const Vector4<V>& v2) noexcept;


    template <class U, class V>
    static void Refract(const Vector4<U>& v1, const Vector4<V>& v2, float refractionIndex, Vector4<T>& result) noexcept;

    template <class U, class V>
    static Vector4 Refract(const Vector4<U>& v1, const Vector4<V>& v2, float refractionIndex) noexcept;


    template <class U>
    static void Transform(const Vector2<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept;

    template <class U>
    static Vector4 Transform(const Vector2<U>& v1, const Quaternion& quat) noexcept;


    template <class U>
    static void Transform(const Vector3<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept;

    template <class U>
    static Vector4 Transform(const Vector3<U>& v1, const Quaternion& quat) noexcept;


    template <class U>
    static void Transform(const Vector4<U>& v1, const Quaternion& quat, Vector4<T>& result) noexcept;

    template <class U>
    static Vector4 Transform(const Vector4<U>& v1, const Quaternion& quat) noexcept;


    template <class U>
    static void Transform(const Vector2<U>& v1, const Matrix& m, Vector4<T>& result) noexcept;

    template <class U>
    static void Transform(_In_reads_(count) const Vector2<U>* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4<T>* resultArray) noexcept;


    template <class U>
    static void Transform(const Vector3<U>& v1, const Matrix& m, Vector4<T>& result) noexcept;

    template <class U>
    static void Transform(_In_reads_(count) const Vector3<U>* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4<T>* resultArray) noexcept;


    template <class U>
    static void Transform(const Vector4<U>& v1, const Matrix& m, Vector4<T>& result) noexcept;

    template <class U>
    static Vector4 Transform(const Vector4<U>& v1, const Matrix& m) noexcept;

    template <class U>
    static void Transform(_In_reads_(count) const Vector4<U>* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4<T>* resultArray) noexcept;

    // Constants
    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;

private:
    [[nodiscard]] static DirectX::XMVECTOR Load(const typename Vector4Base<T>::Base* pSource) noexcept;
    static void Store(typename Vector4Base<T>::Base* pDestination, DirectX::FXMVECTOR v) noexcept;
};

//------------------------------------------------------------------------------
// Binary Operators 
//------------------------------------------------------------------------------

template <class T, class U, class V>
BlocksEngine::Vector4<T> operator+(const BlocksEngine::Vector4<U>& v1, const BlocksEngine::Vector4<V>& v2) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector4<T> operator-(const BlocksEngine::Vector4<U>& v1, const BlocksEngine::Vector4<V>& v2) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector4<T> operator*(const BlocksEngine::Vector4<U>& v1, const BlocksEngine::Vector4<V>& v2) noexcept;

template <class T, class U>
BlocksEngine::Vector4<T> operator*(const BlocksEngine::Vector4<U>& v, float s) noexcept;

template <class T, class U, class V>
BlocksEngine::Vector4<T> operator/(const BlocksEngine::Vector4<U>& v1, const BlocksEngine::Vector4<V>& v2) noexcept;

template <class T, class U>
BlocksEngine::Vector4<T> operator/(const BlocksEngine::Vector4<U>& v, float s) noexcept;

template <class T, class U>
BlocksEngine::Vector4<T> operator*(float s, const BlocksEngine::Vector4<U>& v) noexcept;

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::Zero{0.0f, 0.0f, 0.0f, 0.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::Zero{0, 0, 0, 0};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::Zero{0u, 0u, 0u, 0u};

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::One{1.0f, 1.0f, 1.0f, 1.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::One{1, 1, 1, 1};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::One{1u, 1u, 1u, 1u};

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::UnitX{1.0f, 0.0f, 0.0f, 0.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::UnitX{1, 0, 0, 0};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::UnitX{1u, 0u, 0u, 0u};

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::UnitY{0.0f, 1.0f, 0.0f, 0.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::UnitY{0, 1, 0, 0};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::UnitY{0u, 1u, 0u, 0u};

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::UnitZ{0.0f, 0.0f, 1.0f, 0.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::UnitZ{0, 0, 1, 0};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::UnitZ{0u, 0u, 1u, 0u};

template <>
const BlocksEngine::Vector4<float> BlocksEngine::Vector4<float>::UnitW{0.0f, 0.0f, 0.0f, 1.0f};
template <>
const BlocksEngine::Vector4<int32_t> BlocksEngine::Vector4<int32_t>::UnitW{0, 0, 0, 1};
template <>
const BlocksEngine::Vector4<uint32_t> BlocksEngine::Vector4<uint32_t>::UnitW{0u, 0u, 0u, 1u};

//------------------------------------------------------------------------------
// Storage
//------------------------------------------------------------------------------

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector4<float>::Load(const Vector4Base<float>::Base* pSource) noexcept
{
    return XMLoadFloat4(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector4<int32_t>::Load(const Vector4Base<int32_t>::Base* pSource) noexcept
{
    return XMLoadSInt4(pSource);
}

template <>
inline DirectX::XMVECTOR BlocksEngine::Vector4<uint32_t>::Load(const Vector4Base<uint32_t>::Base* pSource) noexcept
{
    return XMLoadUInt4(pSource);
}

template <>
inline void BlocksEngine::Vector4<float>::Store(Vector4Base<float>::Base* pDestination, DirectX::FXMVECTOR v) noexcept
{
    XMStoreFloat4(pDestination, v);
}

template <>
inline void BlocksEngine::Vector4<int32_t>::Store(Vector4Base<int32_t>::Base* pDestination,
                                                  DirectX::FXMVECTOR v) noexcept
{
    XMStoreSInt4(pDestination, v);
}

template <>
inline void BlocksEngine::Vector4<uint32_t>::Store(Vector4Base<uint32_t>::Base* pDestination,
                                                   DirectX::FXMVECTOR v) noexcept
{
    XMStoreUInt4(pDestination, v);
}
