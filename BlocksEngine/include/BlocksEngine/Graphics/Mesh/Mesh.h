// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Mesh.h

#pragma once
#include <memory>


#include "IndexFormat.h"
#include "MeshTopology.h"
#include "VertexAttribute.h"
#include "BlocksEngine/Exceptions/EngineException.h"
#include "BlocksEngine/Graphics/Bindable.h"
#include "BlocksEngine/Graphics/Mesh/IndexBuffer.h"
#include "BlocksEngine/Graphics/Mesh/VertexBuffer.h"


// TODO: Currently we don't allow changing the layout of the vertex data. This could be changed 
namespace BlocksEngine
{
    class Mesh;
}

class BlocksEngine::Mesh final : public Bindable
{
public:
    using VertexLayoutChangedSignal = boost::signals2::signal<void(const std::vector<VertexAttributeDescriptor>&)>;

    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    explicit Mesh(bool isReadable = false, bool isDynamic = false);


    //------------------------------------------------------------------------------
    // GPU Methods
    //------------------------------------------------------------------------------

    void SetVertexBufferParams(std::vector<VertexAttributeDescriptor> attributeDescriptors);

    template <class T>
    void SetVertexBufferData(std::unique_ptr<std::vector<T>> data);

    void SetIndexBufferParams(int indexCount, IndexFormat indexFormat);

    template <class T>
    void SetIndexBufferData(std::unique_ptr<std::vector<T>> data);

    [[nodiscard]] bool RequiresUpload() const;

    void Upload(const Graphics& gfx);


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------


    [[nodiscard]] int GetVertexCount() const noexcept;

    [[nodiscard]] int GetIndexCount() const noexcept;

    [[nodiscard]] int GetTriangleCount() const;

    [[nodiscard]] size_t GetVertexStride() const;

    [[nodiscard]] IndexFormat GetIndexFormat() const;

    [[nodiscard]] size_t GetIndexStride() const;

    [[nodiscard]] std::array<Vector3<float>, 3> GetTriangle(int index) const;

    [[nodiscard]] std::array<int, 3> GetTriangleIndices(int index) const;

    [[nodiscard]] const void* GetVertex(int index) const;

    [[nodiscard]] const void* GetIndex(int index) const;

    template <class T>
    [[nodiscard]] std::vector<T> GetVertexValue(int index, VertexAttribute attribute) const;

    template <class T>
    [[nodiscard]] T GetIndexValue(int index) const;

    void SetTopology(MeshTopology topology);

    [[nodiscard]] MeshTopology GetTopology() const;


    void Bind(const Graphics& gfx) noexcept override;

    //------------------------------------------------------------------------------
    // Events
    //------------------------------------------------------------------------------

    boost::signals2::connection AddSignalVertexLayoutChanged(const VertexLayoutChangedSignal::slot_type& slot) noexcept;

private:
    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    struct VertexElement
    {
        VertexElement(size_t stride, VertexAttributeDescriptor descriptor);

        VertexElement& operator=(const VertexElement& element);

        size_t stride;
        VertexAttributeDescriptor descriptor;
    };

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    bool isReadable_;
    bool isDynamic_;

    std::unique_ptr<VertexBuffer> pVertexBuffer_{};
    std::unique_ptr<IndexBuffer> pIndexBuffer_{};
    D3D11_PRIMITIVE_TOPOLOGY topology_;

    VertexLayoutChangedSignal vertexLayoutChanged_{};

    // Caching

    robin_hood::unordered_flat_map<VertexAttribute, VertexElement> vertexElements_{};

    //------------------------------------------------------------------------------
    // CPU Data
    //------------------------------------------------------------------------------

    std::shared_ptr<void> vertexData_{nullptr};
    std::shared_ptr<void> indexData_{nullptr};

    //------------------------------------------------------------------------------
    // Data to upload
    //------------------------------------------------------------------------------

    bool didVertexBufferDataChange_{false};
    bool didIndexBufferDataChange_{false};
    std::optional<int> vertexCount_;
    std::optional<int> vertexSize_;
    std::optional<int> indexCount_;
    std::optional<std::vector<VertexAttributeDescriptor>> attributeDescriptors_{std::nullopt};
    std::optional<IndexFormat> indexFormat_;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] std::optional<VertexElement> GetVertexElement(VertexAttribute attribute) const;

    void BindTopology(const Graphics& gfx) const;
};

template <class T>
void BlocksEngine::Mesh::SetVertexBufferData(std::unique_ptr<std::vector<T>> data)
{
    if (GetVertexStride() != sizeof T)
    {
        throw ENGINE_EXCEPTION("The vertex buffer data does not conform to the size of the descriptors");
    }

    vertexSize_ = sizeof T;
    vertexCount_ = data->size();
    vertexData_ = std::move(data->data());
    didVertexBufferDataChange_ = true;
}

template <class T>
void BlocksEngine::Mesh::SetIndexBufferData(std::unique_ptr<std::vector<T>> data)
{
    if (GetIndexStride() != sizeof T)
    {
        throw ENGINE_EXCEPTION("The index buffer data does not conform to the size of the descriptors");
    }

    size_t count = indexCount_ ? indexCount_.value() : pIndexBuffer_->GetCount();
    if (data->size() < count)
    {
        throw ENGINE_EXCEPTION("The data passed contains fewer than the required number of indices");
    }

    indexData_ = std::move(data->data());
    didIndexBufferDataChange_ = true;
}

template <class T>
std::vector<T> BlocksEngine::Mesh::GetVertexValue(const int index, const VertexAttribute attribute) const
{
    const uintptr_t vertexStart = reinterpret_cast<uintptr_t>(GetVertex(index));

    if (const std::optional<VertexElement> element = GetVertexElement(attribute))
    {
        std::vector<T> result(element->descriptor.GetDimension());

        for (int i = 0; i < element->descriptor.GetDimension(); ++i)
        {
            const void* elementPos = reinterpret_cast<void*>(
                vertexStart + element->stride + element->descriptor.GetFormatSize() * i);
            switch (element->descriptor.GetFormat())
            {
            case VertexAttributeFormat::Bool:
                result[i] = static_cast<T>(*static_cast<const bool*>(elementPos));
                break;

            case VertexAttributeFormat::Float32:
                result[i] = static_cast<T>(*static_cast<const float*>(elementPos));
                break;

            case VertexAttributeFormat::UInt32:
                result[i] = static_cast<T>(*static_cast<const uint32_t*>(elementPos));
                break;

            case VertexAttributeFormat::SInt32:
                result[i] = static_cast<T>(*static_cast<const int32_t*>(elementPos));
                break;
            }
        }
        return result;
    }

    throw ENGINE_EXCEPTION("The attribute is not in the mesh");
}

template <class T>
T BlocksEngine::Mesh::GetIndexValue(const int index) const
{
    const void* value = GetIndex(index);
    const IndexFormat format = GetIndexFormat();

    if (format == IndexFormat::UInt16)
    {
        return static_cast<T>(*static_cast<const uint16_t*>(value));
    }
    return static_cast<T>(*static_cast<const uint32_t*>(value));
}
