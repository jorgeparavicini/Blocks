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

namespace BlocksEngine
{
    struct Plane;
    struct Matrix;
    struct Quaternion;
    struct Vector3;
    struct Vector4;

    struct Vector2;
}

struct BlocksEngine::Vector2 : DirectX::XMFLOAT2
{
    Vector2() noexcept;

    constexpr explicit Vector2(float a) noexcept;

    constexpr Vector2(float ix, float iy) noexcept;

    explicit Vector2(_In_reads_(2) const float* pArray) noexcept;

    Vector2(DirectX::FXMVECTOR v) noexcept;

    Vector2(const XMFLOAT2& v) noexcept;

    explicit Vector2(const DirectX::XMVECTORF32& v) noexcept;

    Vector2(const Vector2&) = default;
    Vector2& operator=(const Vector2&) = default;

    Vector2(Vector2&&) = default;
    Vector2& operator=(Vector2&&) = default;

    ~Vector2() = default;

    operator DirectX::XMVECTOR() const noexcept;

    // State getters
    [[nodiscard]] bool IsDirty() const noexcept;
    void ConsumeIsDirty() noexcept;

    // Comparison operators
    bool operator ==(const Vector2& v) const noexcept;
    bool operator !=(const Vector2& v) const noexcept;

    // Assignment operators
    Vector2& operator=(const DirectX::XMVECTORF32& f) noexcept;

    Vector2& operator+=(const Vector2& v) noexcept;
    Vector2& operator-=(const Vector2& v) noexcept;
    Vector2& operator*=(const Vector2& v) noexcept;
    Vector2& operator*=(float s) noexcept;
    Vector2& operator/=(float s) noexcept;

    // Unary operators
    Vector2 operator+() const noexcept;
    Vector2 operator-() const noexcept;

    // Vector operations
    [[nodiscard]] bool InBounds(const Vector2& bounds) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    [[nodiscard]] float Dot(const Vector2& v) const noexcept;
    void Cross(const Vector2& v, Vector2& result) const noexcept;
    [[nodiscard]] Vector2 Cross(const Vector2& v) const noexcept;

    void Normalize() noexcept;
    void Normalize(Vector2& result) const noexcept;

    void Clamp(const Vector2& vMin, const Vector2& vMax) noexcept;
    void Clamp(const Vector2& vMin, const Vector2& vMax, Vector2& result) const noexcept;

    // Static functions
    static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
    static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;

    static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
    static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

    static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
    static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

    static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
    static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;

    static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
    static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

    static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g,
                            Vector2& result) noexcept;
    static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;

    static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t,
                           Vector2& result) noexcept;
    static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4,
                              float t) noexcept;

    static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t,
                        Vector2& result) noexcept;
    static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2,
                           float t) noexcept;

    static void Reflect(const Vector2& iVec, const Vector2& nVec, Vector2& result) noexcept;
    static Vector2 Reflect(const Vector2& iVec, const Vector2& nVec) noexcept;

    static void Refract(const Vector2& iVec, const Vector2& nVec, float refractionIndex, Vector2& result) noexcept;
    static Vector2 Refract(const Vector2& iVec, const Vector2& nVec, float refractionIndex) noexcept;

    static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept;
    static Vector2 Transform(const Vector2& v, const Quaternion& quat) noexcept;

    static void Transform(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
    static Vector2 Transform(const Vector2& v, const Matrix& m) noexcept;
    static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector2* resultArray) noexcept;

    static void Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept;
    static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4* resultArray) noexcept;

    static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
    static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
    static void TransformNormal(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m,
                                _Out_writes_(count) Vector2* resultArray) noexcept;

    // Constants
    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;

private:
    bool isDirty_{true};
};

// Binary operators
BlocksEngine::Vector2 operator+(const BlocksEngine::Vector2& v1, const BlocksEngine::Vector2& v2) noexcept;
BlocksEngine::Vector2 operator-(const BlocksEngine::Vector2& v1, const BlocksEngine::Vector2& v2) noexcept;
BlocksEngine::Vector2 operator*(const BlocksEngine::Vector2& v1, const BlocksEngine::Vector2& v2) noexcept;
BlocksEngine::Vector2 operator*(const BlocksEngine::Vector2& v, float s) noexcept;
BlocksEngine::Vector2 operator/(const BlocksEngine::Vector2& v1, const BlocksEngine::Vector2& v2) noexcept;
BlocksEngine::Vector2 operator/(const BlocksEngine::Vector2& v, float s) noexcept;
BlocksEngine::Vector2 operator*(float s, const BlocksEngine::Vector2& v) noexcept;
