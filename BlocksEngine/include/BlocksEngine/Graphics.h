// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Graphics.h

#pragma once

#include <d3d11_1.h>
#include <d3dcommon.h>
#include <Windows.h>
#include <wrl.h>

namespace BlocksEngine
{
    class Graphics;
}

class BlocksEngine::Graphics
{
public:
    explicit Graphics(HWND hWnd);
    ~Graphics() = default;
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    Graphics(const Graphics&&) = delete;
    Graphics& operator=(const Graphics&&) = delete;

    void CreateDevice();
    void CreateResources();

private:
    D3D_FEATURE_LEVEL featureLevel_;
    Microsoft::WRL::ComPtr<ID3D11Device1> pDevice_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> pContext_;
};
