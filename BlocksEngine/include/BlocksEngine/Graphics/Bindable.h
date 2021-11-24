// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Bindable.h

#pragma once
#include "BlocksEngine/Graphics/Graphics.h"

namespace BlocksEngine
{
    class Bindable;
}

class BlocksEngine::Bindable
{
public:
    Bindable() = default;
    virtual void Bind(const Graphics& gfx) noexcept = 0;
    virtual ~Bindable() = default;

    Bindable(const Bindable&) = delete;
    Bindable& operator=(const Bindable&) = delete;
    Bindable(const Bindable&&) = delete;
    Bindable& operator=(const Bindable&&) = delete;
};
