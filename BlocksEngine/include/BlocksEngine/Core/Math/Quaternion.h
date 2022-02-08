// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Quaternion.h

#pragma once

#include <PxPhysicsAPI.h>
#include <string>

#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    struct Quaternion;
}

struct BlocksEngine::Quaternion : DirectX::XMFLOAT4
{
    Quaternion() noexcept;

    constexpr Quaternion(const float ix, const float iy, const float iz, const float iw) noexcept
        : XMFLOAT4{ix, iy, iz, iw}
    {
    }

    explicit Quaternion(const physx::PxQuat& q) noexcept;

    Quaternion(const Vector3<float>& v, float scalar) noexcept;
    explicit Quaternion(const Vector4<float>& v) noexcept;
    explicit Quaternion(_In_reads_(4) const float* pArray) noexcept;
    Quaternion(DirectX::FXMVECTOR v) noexcept;
    Quaternion(const XMFLOAT4& q) noexcept;
    explicit Quaternion(const DirectX::XMVECTORF32& f) noexcept;

    Quaternion(const Quaternion&) = default;
    Quaternion& operator=(const Quaternion&) = default;

    Quaternion(Quaternion&&) = default;
    Quaternion& operator=(Quaternion&&) = default;

    ~Quaternion() = default;

    operator DirectX::XMVECTOR() const noexcept;
    operator std::string() const noexcept;

    operator physx::PxQuat() const
    {
        return physx::PxQuat{this->x, this->y, this->z, this->w};
    }

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q);
    friend std::wostream& operator<<(std::wostream& os, const Quaternion& q);


    // Comparison operators
    bool operator ==(const Quaternion& q) const noexcept;
    bool operator !=(const Quaternion& q) const noexcept;

    // Assignment operators
    Quaternion& operator=(const DirectX::XMVECTORF32& f) noexcept;

    Quaternion& operator+=(const Quaternion& q) noexcept;
    Quaternion& operator-=(const Quaternion& q) noexcept;
    Quaternion& operator*=(const Quaternion& q) noexcept;
    Quaternion& operator*=(float s) noexcept;
    Quaternion& operator/=(const Quaternion& q) noexcept;

    // Unary operators
    Quaternion operator+() const noexcept;
    Quaternion operator-() const noexcept;

    // Quaternion operations
    [[nodiscard]] float Length() const noexcept;
    [[nodiscard]] float LengthSquared() const noexcept;

    void Normalize() noexcept;
    void Normalize(Quaternion& result) const noexcept;

    void Conjugate() noexcept;
    void Conjugate(Quaternion& result) const noexcept;

    void Inverse(Quaternion& result) const noexcept;

    [[nodiscard]] Vector3<float> EulerAngles() const noexcept;

    [[nodiscard]] float Dot(const Quaternion& q) const noexcept;

    // Static functions
    static Quaternion CreateFromAxisAngle(const Vector3<float>& axis, float angle) noexcept;
    static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
    static Quaternion CreateFromYawPitchRoll(const Vector3<float>& rot) noexcept;
    static Quaternion CreateFromRotationMatrix(const Matrix& m) noexcept;
    static Quaternion Euler(const Vector3<float>& v);
    static Quaternion Euler(float x, float y, float z);

    static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

    static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
    static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

    static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
    static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

    // Constants
    static const Quaternion Identity;

private:
    static Quaternion EulerRadians(const Vector3<float>& v);
    static Quaternion EulerRadians(float x, float y, float z);
    static Vector3<float> NormalizeAngles(const Vector3<float>& v);
    static float NormalizeAngle(float f);
};

// Binary operators
BlocksEngine::Quaternion operator+(const BlocksEngine::Quaternion& q1, const BlocksEngine::Quaternion& q2) noexcept;
BlocksEngine::Quaternion operator-(const BlocksEngine::Quaternion& q1, const BlocksEngine::Quaternion& q2) noexcept;
BlocksEngine::Quaternion operator*(const BlocksEngine::Quaternion& q1, const BlocksEngine::Quaternion& q2) noexcept;
BlocksEngine::Quaternion operator*(const BlocksEngine::Quaternion& q, float s) noexcept;
BlocksEngine::Quaternion operator/(const BlocksEngine::Quaternion& q1, const BlocksEngine::Quaternion& q2) noexcept;
BlocksEngine::Quaternion operator*(float s, const BlocksEngine::Quaternion& q) noexcept;
