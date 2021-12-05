// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Matrix3x3.h

#pragma once

#include <DirectXMath.h>

#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    struct Matrix3X3;
}

struct BlocksEngine::Matrix3X3 : DirectX::XMFLOAT3X3
{
    //------------------------------------------------------------------------------
    // Constructors, Destructors & Copy, Move
    //------------------------------------------------------------------------------

    Matrix3X3() noexcept;

    constexpr Matrix3X3(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22) noexcept;

    explicit Matrix3X3(const Vector3<float>& r0, const Vector3<float>& r1, const Vector3<float>& r2) noexcept;

    Matrix3X3(const XMFLOAT3X3& m) noexcept;

    explicit Matrix3X3(_In_reads_(9) const float* pArray) noexcept;

    Matrix3X3(DirectX::CXMMATRIX m) noexcept;

    Matrix3X3(const Matrix3X3&) = default;
    Matrix3X3& operator=(const Matrix3X3&) = default;

    Matrix3X3(Matrix3X3&&) = default;
    Matrix3X3& operator=(Matrix3X3&&) = default;

    ~Matrix3X3() = default;


    //------------------------------------------------------------------------------
    // operators
    //------------------------------------------------------------------------------

    operator DirectX::XMMATRIX() const noexcept;

    Vector3<float> operator[](int index) const;

    // Comparison operators
    bool operator==(const Matrix3X3& m) const noexcept;
    bool operator!=(const Matrix3X3& m) const noexcept;

    // Assignment operators
    Matrix3X3& operator+=(const Matrix3X3& m) noexcept;
    Matrix3X3& operator-=(const Matrix3X3& m) noexcept;
    Matrix3X3& operator*=(const Matrix3X3& m) noexcept;
    Matrix3X3& operator*=(float s) noexcept;
    Matrix3X3& operator/=(float s) noexcept;

    Matrix3X3& operator/=(const Matrix3X3& m) noexcept;

    // Unary operators
    Matrix3X3 operator+() const noexcept;
    Matrix3X3 operator-() const noexcept;

    // Properties
    [[nodiscard]] Vector3<float> Up() const noexcept;
    void Up(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Down() const noexcept;
    void Down(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Right() const noexcept;
    void Right(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Left() const noexcept;
    void Left(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Forward() const noexcept;
    void Forward(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Backward() const noexcept;
    void Backward(const Vector3<float>& v) noexcept;

    [[nodiscard]] Vector3<float> Translation() const noexcept;
    void Translation(const Vector3<float>& v) noexcept;

    // Matrix operations
    bool Decompose(Vector3<float>& scale, Quaternion& rotation, Vector3<float>& translation) const noexcept;

    [[nodiscard]] Matrix3X3 Transpose() const noexcept;
    void Transpose(Matrix3X3& result) const noexcept;

    [[nodiscard]] Matrix3X3 Invert() const noexcept;
    void Invert(Matrix3X3& result) const noexcept;

    [[nodiscard]] float Determinant() const noexcept;

    // Static functions
    static Matrix3X3 CreateBillboard(
        const Vector3<float>& object, const Vector3<float>& cameraPosition, const Vector3<float>& cameraUp,
        _In_opt_ const Vector3<float>* cameraForward = nullptr) noexcept;

    static Matrix3X3 CreateConstrainedBillboard(
        const Vector3<float>& object,
        const Vector3<float>& cameraPosition,
        const Vector3<float>& rotateAxis,
        _In_opt_ const Vector3<float>* cameraForward = nullptr,
        _In_opt_ Vector3<float>* objectForward = nullptr) noexcept;

    static Matrix3X3 CreateTranslation(const Vector3<float>& position) noexcept;
    static Matrix3X3 CreateTranslation(float x, float y, float z) noexcept;

    static Matrix3X3 CreateScale(const Vector3<float>& scales) noexcept;
    static Matrix3X3 CreateScale(float xs, float ys, float zs) noexcept;
    static Matrix3X3 CreateScale(float scale) noexcept;

    static Matrix3X3 CreateRotationX(float radians) noexcept;
    static Matrix3X3 CreateRotationY(float radians) noexcept;
    static Matrix3X3 CreateRotationZ(float radians) noexcept;

    static Matrix3X3 CreateFromAxisAngle(const Vector3<float>& axis, float angle) noexcept;

    static Matrix3X3 CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane,
                                                  float farPlane) noexcept;
    static Matrix3X3 CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept;
    static Matrix3X3 CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane,
                                                float farPlane) noexcept;
    static Matrix3X3 CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
    static Matrix3X3 CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane,
                                                 float zFarPlane) noexcept;

    static Matrix3X3 CreateLookAt(const Vector3<float>& position, const Vector3<float>& target,
                                  const Vector3<float>& up) noexcept;

    static Matrix3X3 CreateFromQuaternion(const Quaternion& quat) noexcept;

    static Matrix3X3 CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;

    static void Lerp(const Matrix3X3& m1, const Matrix3X3& m2, float t, Matrix3X3& result) noexcept;
    static Matrix3X3 Lerp(const Matrix3X3& m1, const Matrix3X3& m2, float t) noexcept;

    static void Transform(const Matrix3X3& m, const Quaternion& rotation, Matrix3X3& result) noexcept;
    static Matrix3X3 Transform(const Matrix3X3& m, const Quaternion& rotation) noexcept;

    // Constants
    static const Matrix3X3 Identity;
};
