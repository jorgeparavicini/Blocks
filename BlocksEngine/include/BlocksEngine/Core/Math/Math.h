// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Math.h

#pragma once

#include <algorithm>

namespace BlocksEngine
{
    struct Plane;
    struct Matrix;
    struct Quaternion;

    template <class T = float>
    struct Vector2;

    template <class T = float>
    struct Vector3;

    template <class T = float>
    struct Vector4;
}

#include "BlocksEngine/Core/Math/Matrix.h"
#include "BlocksEngine/Core/Math/Plane.h"
#include "BlocksEngine/Core/Math/Quaternion.h"
#include "BlocksEngine/Core/Math/Vector2.h"
#include "BlocksEngine/Core/Math/Vector3.h"
#include "BlocksEngine/Core/Math/Vector4.h"

namespace BlocksEngine::Math
{
    constexpr float DEG_TO_RAD = 0.0174533f;
    constexpr float PI = 3.14159265359;
    constexpr float EPSILON = std::numeric_limits<float>::epsilon();

    inline float Max(const float a, const float b, const float c)
    {
        return std::max(std::max(a, b), c);
    }

    inline float Min(const float a, const float b, const float c)
    {
        return std::min(std::min(a, b), c);
    }
}
