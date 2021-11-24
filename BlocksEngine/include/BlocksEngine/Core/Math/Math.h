// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Math.h

#pragma once

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


    class Math;
}

#include "BlocksEngine/Core/Math/Matrix.h"
#include "BlocksEngine/Core/Math/Plane.h"
#include "BlocksEngine/Core/Math/Quaternion.h"
#include "BlocksEngine/Core/Math/Vector2.h"
#include "BlocksEngine/Core/Math/Vector3.h"
#include "BlocksEngine/Core/Math/Vector4.h"


class BlocksEngine::Math
{
public:
    static const float DegToRad;
};
