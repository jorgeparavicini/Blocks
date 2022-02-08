#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/Topology.h"

using namespace BlocksEngine;

Topology::Topology(const D3D11_PRIMITIVE_TOPOLOGY topology)
    : topology_{topology}
{
}

void Topology::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().IASetPrimitiveTopology(topology_);
}

std::shared_ptr<Topology> Topology::TriangleList = std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
