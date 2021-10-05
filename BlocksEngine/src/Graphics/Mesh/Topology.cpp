#include "BlocksEngine/pch.h"
#include "BlocksEngine/Topology.h"

BlocksEngine::Topology::Topology(const D3D11_PRIMITIVE_TOPOLOGY topology)
    : topology_{topology}
{
}

void BlocksEngine::Topology::Bind(const Graphics& gfx) noexcept
{
    gfx.GetContext().IASetPrimitiveTopology(topology_);
}

std::shared_ptr<BlocksEngine::Topology> BlocksEngine::Topology::TriangleList = std::make_shared<Topology>(
    D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
