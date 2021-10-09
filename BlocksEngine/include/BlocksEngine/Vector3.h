// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vector3.h

// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

namespace BlocksEngine
{
    class Vector3;
}

class BlocksEngine::Vector3
{
public:
    Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);

    [[nodiscard]] float X() const noexcept;
    [[nodiscard]] float Y() const noexcept;
    [[nodiscard]] float Z() const noexcept;
    [[nodiscard]] float Magnitude() const noexcept;
    [[nodiscard]] float SqrMagnitude() const noexcept;
    [[nodiscard]] Vector3 Normalized() const noexcept;

    [[nodiscard]] float operator[](int i) const;

    static Vector3 One();
    static Vector3 Zero();
    static Vector3 Back();
    static Vector3 Down();
    static Vector3 Forward();
    static Vector3 Left();
    static Vector3 Right();
    static Vector3 Up();

private:
    float x_;
    float y_;
    float z_;
};
