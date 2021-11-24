// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Plane.h

#pragma once

#include <DirectXMath.h>

#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    struct Plane;
}

struct BlocksEngine::Plane : DirectX::XMFLOAT4
{
    Plane() noexcept;
    constexpr Plane(float ix, float iy, float iz, float iw) noexcept;
    Plane(const Vector3<float>& normal, float d) noexcept;
    Plane(const Vector3<float>& point1, const Vector3<float>& point2, const Vector3<float>& point3) noexcept;
    Plane(const Vector3<float>& point, const Vector3<float>& normal) noexcept;
    explicit Plane(const Vector4<float>& v) noexcept;
    explicit Plane(_In_reads_(4) const float* pArray) noexcept;
    Plane(DirectX::FXMVECTOR v) noexcept;
    Plane(const XMFLOAT4& p) noexcept;
    explicit Plane(const DirectX::XMVECTORF32& f) noexcept;

    Plane(const Plane&) = default;
    Plane& operator=(const Plane&) = default;

    Plane(Plane&&) = default;
    Plane& operator=(Plane&&) = default;

    ~Plane() = default;

    // State getters
    [[nodiscard]] bool IsDirty() const noexcept;
    void ConsumeIsDirty() noexcept;

    operator DirectX::XMVECTOR() const noexcept;

    // Comparison operators
    bool operator ==(const Plane& p) const noexcept;
    bool operator !=(const Plane& p) const noexcept;

    // Assignment operators
    Plane& operator=(const DirectX::XMVECTORF32& f) noexcept;

    // Properties
    [[nodiscard]] Vector3<float> Normal() const noexcept;

    void Normal(const Vector3<float>& normal) noexcept;

    [[nodiscard]] float D() const noexcept;
    void D(float d) noexcept;

    // Plane operations
    void Normalize() noexcept;
    void Normalize(Plane& result) const noexcept;

    [[nodiscard]] float Dot(const Vector4<float>& v) const noexcept;
    [[nodiscard]] float DotCoordinate(const Vector3<float>& position) const noexcept;
    [[nodiscard]] float DotNormal(const Vector3<float>& normal) const noexcept;

    // Static functions
    static void Transform(const Plane& plane, const Matrix& m, Plane& result) noexcept;
    static Plane Transform(const Plane& plane, const Matrix& m) noexcept;

    static void Transform(const Plane& plane, const Quaternion& rotation, Plane& result) noexcept;
    static Plane Transform(const Plane& plane, const Quaternion& rotation) noexcept;

private:
    bool isDirty_{true};
};
