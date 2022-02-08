// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: PlayerDebugs.h

#pragma once
#include <d2d1.h>
#include <wrl/client.h>

#include "BlocksEngine/Core/Components/Component.h"

namespace Blocks
{
    class PlayerDebugs;
}

class Blocks::PlayerDebugs final : public BlocksEngine::Component
{
public:
    void Start() override;
    void Draw2D() override;

private:
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pTextBrush_;
    Microsoft::WRL::ComPtr<IDWriteTextFormat> pTextFormat_;

    std::weak_ptr<BlocksEngine::Transform> cameraTransform_;
};
