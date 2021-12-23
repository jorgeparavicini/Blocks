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

    vertexElements_.clear();
    size_t size{0};
    for (auto& attrDesc : attributeDescriptors)
    {
        vertexElements_[attrDesc.GetAttribute()] = VertexElement{size, attrDesc};
        size += attrDesc.GetFormatSize() * attrDesc.GetDimension();
    }

    vertexLayoutChanged_(attributeDescriptors_.value());
}

void Mesh::SetIndexBufferParams(int indexCount, IndexFormat indexFormat)
{
    indexCount_ = indexCount;
    indexFormat_ = indexFormat;
}

int Mesh::GetVertexCount() const noexcept
{
    return vertexCount_ ? vertexCount_.value() : pVertexBuffer_->GetCount();
}

int Mesh::GetIndexCount() const noexcept
{
    return indexCount_ ? indexCount_.value() : pIndexBuffer_->GetCount();
}

int Mesh::GetTriangleCount() const
{
    const int count = GetIndexCount();
    // TODO: Verify that this is even an error. Maybe the system truncates it to a multiple of 3.
    assert("The index count is not a multiple of 3" && count % 3 == 0);
    return count / 3;
}

size_t Mesh::GetVertexStride() const
{
    // TODO: This really needs to be cached
    if (attributeDescriptors_)
    {
        size_t size{0};
        for (auto& attrDesc : attributeDescriptors_.value())
        {
            size += attrDesc.GetFormatSize() * attrDesc.GetDimension();
        }
        return size;
    }

    if (!pVertexBuffer_)
    {
        throw ENGINE_EXCEPTION("Must set the attributes descriptors before trying to get the vertex stride");
    }

    return pVertexBuffer_->GetSize();
}

IndexFormat Mesh::GetIndexFormat() const
{
    if (indexFormat_)
    {
        return indexFormat_.value();
    }

    if (!pIndexBuffer_)
    {
        throw ENGINE_EXCEPTION("Must set the index format before trying to get the index stride");
    }

    const auto size = pIndexBuffer_->GetSize();
    if (size == 2)
    {
        return IndexFormat::UInt16;
    }
    if (size == 4)
    {
        return IndexFormat::UInt32;
    }

    throw ENGINE_EXCEPTION("Invalid index format with size: " + std::to_string(size));
}

size_t Mesh::GetIndexStride() const
{
    // TODO: Caching
    if (indexFormat_)
    {
        return indexFormat_.value() == IndexFormat::UInt16 ? 2 : 4;
    }

    if (!pIndexBuffer_)
    {
        throw ENGINE_EXCEPTION("Must set the index format before trying to get the index stride");
    }

    return pIndexBuffer_->GetSize();
}

std::array<Vector3<float>, 3> Mesh::GetTriangle(const int index) const
{
    assert(index < GetTriangleCount());
    std::array<Vector3<float>, 3> result;
    for (int i = 0; i < 3; ++i)
    {
        auto vertex = GetVertexValue<float>(index * 3 + i, VertexAttribute::Position);
        if (vertex.size() != 3)
        {
            throw ENGINE_EXCEPTION("The position vertices must be 3-dimensional");
        }
        result[i] = Vector3{vertex[0], vertex[1], vertex[2]};
    }

    return result;
}

std::array<int, 3> Mesh::GetTriangleIndices(const int index) const
{
    assert(index < GetTriangleCount());
    return {
        GetIndexValue<int>(index * 3 + 0),
        GetIndexValue<int>(index * 3 + 1),
        GetIndexValue<int>(index * 3 + 2)
    };
}

const void* Mesh::GetVertex(const int index) const
{
    if (!isReadable_)
    {
        throw ENGINE_EXCEPTION("Only readable meshes can be read from the cpu");
    }

    const uintptr_t start = reinterpret_cast<uintptr_t>(vertexData_.get());
    return reinterpret_cast<void*>(start + index * GetVertexStride());
}

const void* Mesh::GetIndex(const int index) const
{
    if (!isReadable_)
    {
        throw ENGINE_EXCEPTION("Only readable meshes can be read from the cpu");
    }

    const uintptr_t start = reinterpret_cast<uintptr_t>(indexData_.get());
    return reinterpret_cast<void*>(start + index * GetIndexStride());
}

std::optional<Mesh::VertexElement> Mesh::GetVertexElement(const VertexAttribute attribute) const
{
    const auto search = vertexElements_.find(attribute);
    if (search != vertexElements_.end())
    {
        return search->second;
    }

    return std::nullopt;
}

bool Mesh::RequiresUpload() const
{
    // TODO: Basically these 2 are not needed. Whenever any optional variable is set the mesh needs to be uploaded.
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

    // Reset data that needed to be uploaded
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


void Mesh::BindTopology(const Graphics& gfx) const
{
    gfx.GetContext().IASetPrimitiveTopology(topology_);
}

boost::signals2::connection Mesh::AddSignalVertexLayoutChanged(
    const VertexLayoutChangedSignal::slot_type& slot) noexcept
{
    return vertexLayoutChanged_.connect(slot);
}

Mesh::VertexElement::VertexElement(const size_t stride, VertexAttributeDescriptor descriptor)
    : stride{stride}, descriptor{descriptor}
{
}

Mesh::VertexElement& Mesh::VertexElement::operator=(const VertexElement& element)
{
    if (this != &element)
    {
        this->descriptor = element.descriptor;
        this->stride = element.stride;
    }
    return *this;
}
