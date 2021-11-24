// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: LoadingScreen.h

#pragma once
#include <d2d1.h>
#include <wrl/client.h>

#include "BlocksEngine/Core/Components/Component.h"

namespace Blocks
{
    class LoadingScreen;
}

class Blocks::LoadingScreen final : public BlocksEngine::Component
{
public:
    LoadingScreen(std::weak_ptr<BlocksEngine::Actor> actor);

    void LevelLoaded();

    void Draw2D() override;

private:
    bool isLoading_{true};

    // 2D Rendering resources
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush_;

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pTextBrush_;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> pTextFormat_;
};
