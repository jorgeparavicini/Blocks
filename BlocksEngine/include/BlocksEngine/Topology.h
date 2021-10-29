// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Topology.h

#pragma once
#include "BlocksEngine/Bindable.h"

namespace BlocksEngine
{
    class Topology;
}

class BlocksEngine::Topology final : public Bindable
{
public:
    explicit Topology(D3D11_PRIMITIVE_TOPOLOGY topology);
    void Bind(const Graphics& gfx) noexcept override;

    static std::shared_ptr<Topology> TriangleList;

protected:
    D3D11_PRIMITIVE_TOPOLOGY topology_;
};
