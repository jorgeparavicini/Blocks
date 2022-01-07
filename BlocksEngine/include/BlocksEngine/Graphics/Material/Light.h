// 
// Copyright (c) 2022, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Light.h

#pragma once

#include <DirectXMath.h>
#include <Windows.h>

namespace BlocksEngine
{
    struct Light;
}

struct BlocksEngine::Light
{
    Light()
    {
        ZeroMemory(this, sizeof Light);
    }

    DirectX::XMFLOAT3 dir;
    float pad;
    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
};
