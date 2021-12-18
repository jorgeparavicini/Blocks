#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/Mesh.h"

using namespace BlocksEngine;


Mesh::Mesh(const bool isReadable, const bool isDynamic)
    : isReadable_{isReadable}, isDynamic_{isDynamic}, topology_{D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST}
{
}

void Mesh::SetVertexBufferParams(std::vector<VertexAttributeDescriptor> attributeDescriptors)
{
    attributeDescriptors_ = std::move(attributeDescriptors);
    vertexLayoutChanged_(attributeDescriptors_.value());
}

void Mesh::SetIndexBufferParams(int indexCount, IndexFormat indexFormat)
{
    indexCount_ = indexCount;
    indexFormat_ = indexFormat;
}

int Mesh::GetVertexCount() const noexcept
{
}

int Mesh::GetTriangleCount() const
{
}

size_t Mesh::GetVerticesStride() const
{
}

size_t Mesh::GetVerticesNormalsStride() const
{
}

size_t Mesh::GetIndicesStride() const
{
}

const void* Mesh::GetVertex(size_t index) const
{
}

const void* Mesh::GetVertexNormal(size_t index) const
{
}

const void* Mesh::GetIndex(size_t index) const
{
}

bool Mesh::RequiresUpload() const
{
    return didVertexBufferDataChange_ || didIndexBufferDataChange_;
}

void Mesh::Upload(const Graphics& gfx)
{
    // TODO: Currently we just yeet the old buffers away but we can often reuse the existing buffers if they are large enough
    // They have to be dynamic for that tho
    if (didVertexBufferDataChange_)
    {
        size_t vertexCount = vertexCount_ ? vertexCount_.value() : pVertexBuffer_->GetCount();
        size_t vertexSize = vertexSize_ ? vertexSize_.value() : pVertexBuffer_->GetSize();
        pVertexBuffer_ = std::make_unique<VertexBuffer>(gfx, vertexData_, vertexSize, vertexCount, false);
    }

    if (didIndexBufferDataChange_)
    {
        int indexCount = indexCount_ ? indexCount_.value() : pIndexBuffer_->GetCount();
        int indexSize = indexFormat_ ? (indexFormat_.value() == IndexFormat::UInt16 ? 1 : 2) : pIndexBuffer_->GetSize();
        pIndexBuffer_ = std::make_unique<IndexBuffer>(gfx, indexData_, indexSize, indexCount, false);
    }

    // TODO: Set index count | format if those changed but the data didn't

    // TODO: Generally all of this is like a lego set made out of paper
    // Destroy CPU Data if the mesh is not readable
    if (!isReadable_)
    {
        vertexData_ = nullptr;
        indexData_ = nullptr;
    }

    attributeDescriptors_ = std::nullopt;
    indexCount_ = std::nullopt;
    indexFormat_ = std::nullopt;
    didVertexBufferDataChange_ = false;
    didIndexBufferDataChange_ = false;
}


void Mesh::SetTopology(const MeshTopology topology)
{
    switch (topology)
    {
    case MeshTopology::Points:
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        break;

    case MeshTopology::Lines:
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        break;

    case MeshTopology::LineStrip:
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        break;

    case MeshTopology::Triangles:
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;

    case MeshTopology::TriangleStrip:
        topology_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        break;
    }
}

MeshTopology Mesh::GetTopology() const
{
    switch (topology_)
    {
    case D3D11_PRIMITIVE_TOPOLOGY_POINTLIST:
        return MeshTopology::Points;

    case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
        return MeshTopology::Lines;

    case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
        return MeshTopology::LineStrip;

    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
        return MeshTopology::Triangles;

    case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
        return MeshTopology::TriangleStrip;

    default:
        throw std::out_of_range("The internal mesh topology is in an invalid state");
    }
}

void Mesh::Bind(const Graphics& gfx) noexcept
{
    BindTopology(gfx);
    pVertexBuffer_->Bind(gfx);
    pIndexBuffer_->Bind(gfx);
}

boost::signals2::connection Mesh::AddSignalVertexLayoutChanged(
    const VertexLayoutChangedSignal::slot_type& slot) noexcept
{
    return vertexLayoutChanged_.connect(slot);
}

void Mesh::BindTopology(const Graphics& gfx) const
{
    gfx.GetContext().IASetPrimitiveTopology(topology_);
}
