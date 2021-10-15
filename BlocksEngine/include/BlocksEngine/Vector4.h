// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vector4.h

#pragma once

namespace BlocksEngine
{
    struct Quaternion;
    struct Matrix;
    struct Plane;
    struct Vector2;
    struct Vector3;

    struct Vector4;
}

struct BlocksEngine::Vector4 : DirectX::XMFLOAT4
{
    Vector4() noexcept;

    constexpr explicit Vector4(float a) noexcept;

    constexpr Vector4(float x, float y, float z, float w) noexcept;

    explicit Vector4(_In_reads_(4) const float* pArray) noexcept;

    Vector4(DirectX::FXMVECTOR v) noexcept;

    Vector4(const XMFLOAT4& v) noexcept;

    explicit Vector4(const DirectX::XMVECTORF32& f) noexcept;

    Vector4(const Vector4&) = default;
    Vector4& operator=(const Vector4&) = default;

    Vector4(Vector4&&) = default;
    Vector4& operator=(Vector4&&) = default;

    operator DirectX::XMVECTOR() const noexcept;

    // State getters
    [[nodiscard]] bool IsDirty() const noexcept;
    void ConsumeIsDirty() noexcept;

    // Comparison operators
    bool operator ==(const Vector4& v) const noexcept;
    bool operator !=(const Vector4& v) const noexcept;

    // Assignment operators
    Vector4& operator=(const DirectX::XMVECTORF32& f) noexcept;
    Vector4& operator+=(const Vector4& v) noexcept;
    Vector4& operator-=(const Vector4& v) noexcept;
    Vector4& operator*=(const Vector4& v) noexcept;
    Vector4& operator*=(float s) noexcept;
    Vector4& operator/=(float s) noexcept;

    // Unary operators
    Vector4 operator+() const noexcept;
    Vector4 operator-() const noexcept;

    // Vector operations
    [[nodiscard]] bool InBounds(const Vector4& bounds) const noexcept;

    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    [[nodiscard]] float Dot(const Vector4& v) const noexcept;
    void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
    [[nodiscard]] Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;

    void Normalize() noexcept;
    void Normalize(Vector4& result) const noexcept;

    void Clamp(const Vector4& vMin, const Vector4& vMax) noexcept;
    void Clamp(const Vector4& vMin, const Vector4& vMax, Vector4& result) const noexcept;

    // Static functions
    static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
    static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

    static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
    static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

    static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
    static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

    static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
    static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;

    static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
    static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

    static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g,
                            Vector4& result) noexcept;
    static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;

    static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t,
                           Vector4& result) noexcept;
    static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4,
                              float t) noexcept;

    static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t,
                        Vector4& result) noexcept;
    static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2,
                           float t) noexcept;

    static void Reflect(const Vector4& iVec, const Vector4& nVec, Vector4& result) noexcept;
    static Vector4 Reflect(const Vector4& iVec, const Vector4& nVec) noexcept;

    static void Refract(const Vector4& iVec, const Vector4& nVec, float refractionIndex, Vector4& result) noexcept;
    static Vector4 Refract(const Vector4& iVec, const Vector4& nVec, float refractionIndex) noexcept;

    static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept;
    static Vector4 Transform(const Vector2& v, const Quaternion& quat) noexcept;

    static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept;
    static Vector4 Transform(const Vector3& v, const Quaternion& quat) noexcept;

    static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept;
    static Vector4 Transform(const Vector4& v, const Quaternion& quat) noexcept;

    static void Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept;
    static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;
    static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m,
                          _Out_writes_(count) Vector4* resultArray) noexcept;

    // Constants
    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;

private:
    bool isDirty_{true};
};

BlocksEngine::Vector4 operator+(const BlocksEngine::Vector4& v1, const BlocksEngine::Vector4& v2) noexcept;
BlocksEngine::Vector4 operator-(const BlocksEngine::Vector4& v1, const BlocksEngine::Vector4& v2) noexcept;
BlocksEngine::Vector4 operator*(const BlocksEngine::Vector4& v1, const BlocksEngine::Vector4& v2) noexcept;
BlocksEngine::Vector4 operator*(const BlocksEngine::Vector4& v, float s) noexcept;
BlocksEngine::Vector4 operator/(const BlocksEngine::Vector4& v1, const BlocksEngine::Vector4& v2) noexcept;
BlocksEngine::Vector4 operator/(const BlocksEngine::Vector4& v, float s) noexcept;
BlocksEngine::Vector4 operator*(float s, const BlocksEngine::Vector4& v) noexcept;
