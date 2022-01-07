// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Vertex.h

#pragma once

#include <DirectXMath.h>

namespace BlocksEngine
{
    struct Vertex
    {
        Vector3<float> pos;
        Vector2<float> texCoord;
        Vector3<float> normal;

        unsigned int texIndex;
    };
}
