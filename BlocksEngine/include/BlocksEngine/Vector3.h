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

#include "BlocksEngine/Matrix.h"

// This file is based on the DirectXTK SimpleMath header and extended for engine uses.

namespace BlocksEngine
{
    struct Vector3;
}

struct BlocksEngine::Vector3 : DirectX::XMFLOAT3
{
    Vector3() noexcept;

    constexpr Vector3(const float a) noexcept
        : XMFLOAT3{a, a, a}
    {
    }

    constexpr Vector3(const float x, const float y, const float z) noexcept
        : XMFLOAT3{x, y, z}
    {
    }

    explicit Vector3(_In_reads_(3) const float* pArray) noexcept;
    Vector3(DirectX::FXMVECTOR v) noexcept;
    Vector3(const XMFLOAT3& v) noexcept;
    explicit Vector3(const DirectX::XMVECTORF32& v) noexcept;

    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;

    Vector3(Vector3&&) = default;
    Vector3& operator=(Vector3&&) = default;

    ~Vector3() = default;

    operator DirectX::XMVECTOR() const noexcept;

    // Comparison operators
    bool operator==(const Vector3& v) const noexcept;
    bool operator!=(const Vector3& v) const noexcept;

    // Assignment operators
    Vector3& operator=(const DirectX::XMVECTORF32& v) noexcept;
    Vector3& operator+=(const Vector3& v) noexcept;
    Vector3& operator-=(const Vector3& v) noexcept;
    Vector3& operator*=(const Vector3& v) noexcept;
    Vector3& operator*=(float s) noexcept;
    Vector3& operator/=(float s) noexcept;

    // Unary operators
    Vector3 operator+() const noexcept;
    Vector3 operator-() const noexcept;

    // Hashing
    friend std::size_t hash_value(const Vector3& v);

    // Vector operations
    [[nodiscard]] bool InBounds(const Vector3& bounds) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    [[nodiscard]] float Dot(const Vector3& v) const noexcept;
    void Cross(const Vector3& v, Vector3& result) const noexcept;
    [[nodiscard]] Vector3 Cross(const Vector3& v) const noexcept;

    void Normalize() noexcept;
    void Normalize(Vector3& result) const noexcept;

    void Clamp(const Vector3& vMin, const Vector3& vMax) noexcept;
    void Clamp(const Vector3& vMin, const Vector3& vMax, Vector3& result) const noexcept;

    // Static functions
    static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
    static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

    static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
    static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

    static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
    static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

    static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
    static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;

    static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
    static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

    static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g,
                            Vector3& result) noexcept;
    static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;

    static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t,
                           Vector3& result) noexcept;
    static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4,
                              float t) noexcept;

    static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t,
                        Vector3& result) noexcept;
    static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2,
                           float t) noexcept;

    static void Reflect(const Vector3& iVec, const Vector3& nVec, Vector3& result) noexcept;
    static Vector3 Reflect(const Vector3& iVec, const Vector3& nVec) noexcept;

    static void Refract(const Vector3& iVec, const Vector3& nVec, float refractionIndex, Vector3& result) noexcept;
    static Vector3 Refract(const Vector3& iVec, const Vector3& nVec, float refractionIndex) noexcept;

    static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept;
    static Vector3 Transform(const Vector3& v, const Quaternion& quat) noexcept;

    static void Transform(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
    static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
    static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector3* resultArray) noexcept;

    static void Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept;
    static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4* resultArray) noexcept;

    static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
    static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m,
                                _Out_writes_(count) Vector3* resultArray) noexcept;

    // Constants
    static const Vector3 Zero;
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
};

// Binary operators
BlocksEngine::Vector3 operator+(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept;
BlocksEngine::Vector3 operator-(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept;
BlocksEngine::Vector3 operator*(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept;
BlocksEngine::Vector3 operator*(const BlocksEngine::Vector3& v, float s) noexcept;
BlocksEngine::Vector3 operator*(const BlocksEngine::Quaternion& q, const BlocksEngine::Vector3& v) noexcept;
BlocksEngine::Vector3 operator/(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept;
BlocksEngine::Vector3 operator/(const BlocksEngine::Vector3& v, float s) noexcept;
BlocksEngine::Vector3 operator*(float s, const BlocksEngine::Vector3& v) noexcept;
