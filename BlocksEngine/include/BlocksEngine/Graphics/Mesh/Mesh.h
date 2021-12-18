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


// TODO: Currently we don't allow changing the layout of the vertex data. This could be changed though

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

    Mesh(bool isReadable = false, bool isDynamic = false);


    //------------------------------------------------------------------------------
    // GPU Methods
    //------------------------------------------------------------------------------

    void SetVertexBufferParams(std::vector<VertexAttributeDescriptor> attributeDescriptors);

    template <class T>
    void SetVertexBufferData(std::unique_ptr<std::vector<T>> data);

    void SetIndexBufferParams(int indexCount, IndexFormat indexFormat);

    template <class T>
    void SetIndexBufferData(std::unique_ptr<std::vector<T>> data);


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------


    [[nodiscard]] int GetVertexCount() const noexcept;

    [[nodiscard]] int GetTriangleCount() const;

    [[nodiscard]] size_t GetVerticesStride() const;

    [[nodiscard]] size_t GetVerticesNormalsStride() const;

    [[nodiscard]] size_t GetIndicesStride() const;

    [[nodiscard]] const void* GetVertex(size_t index) const;

    [[nodiscard]] const void* GetVertexNormal(size_t index) const;

    [[nodiscard]] const void* GetIndex(size_t index) const;

    void SetTopology(MeshTopology topology);

    [[nodiscard]] MeshTopology GetTopology() const;

    [[nodiscard]] bool RequiresUpload() const;

    void Upload(const Graphics& gfx);


    void Bind(const Graphics& gfx) noexcept override;

    //------------------------------------------------------------------------------
    // Events
    //------------------------------------------------------------------------------

    boost::signals2::connection AddSignalVertexLayoutChanged(const VertexLayoutChangedSignal::slot_type& slot) noexcept;

private:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    bool isReadable_;
    bool isDynamic_;

    std::unique_ptr<VertexBuffer> pVertexBuffer_{};
    std::unique_ptr<IndexBuffer> pIndexBuffer_{};
    D3D11_PRIMITIVE_TOPOLOGY topology_;

    VertexLayoutChangedSignal vertexLayoutChanged_{};

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

    void BindTopology(const Graphics& gfx) const;
};

template <class T>
void BlocksEngine::Mesh::SetVertexBufferData(std::unique_ptr<std::vector<T>> data)
{
    if (attributeDescriptors_)
    {
        size_t size{0};
        for (auto& vertexAttributeDescriptor : attributeDescriptors_.value())
        {
            size += vertexAttributeDescriptor.GetFormatSize() * vertexAttributeDescriptor.GetDimension();
        }
        if (sizeof T != size)
        {
            throw ENGINE_EXCEPTION("The vertex buffer data does not conform to the size of the descriptors");
        }
    }
    else if (pVertexBuffer_)
    {
        if (pVertexBuffer_->GetSize() != sizeof T)
        {
            throw ENGINE_EXCEPTION("The vertex buffer data does not conform to the size of the vertex buffer");
        }
    }
    else
    {
        throw ENGINE_EXCEPTION(
            "Can not set the vertex buffer data without setting descriptors first or an existing vertex buffer");
    }

    vertexSize_ = sizeof T;
    vertexCount_ = data->size();
    vertexData_ = std::move(data->data());
    didVertexBufferDataChange_ = true;
}

template <class T>
void BlocksEngine::Mesh::SetIndexBufferData(std::unique_ptr<std::vector<T>> data)
{
    if (indexFormat_)
    {
        const size_t size = indexFormat_ == IndexFormat::UInt16 ? 1 : 2;
        if (sizeof T != size)
        {
            throw ENGINE_EXCEPTION("The index buffer data does not conform to the size of the descriptors");
        }
    }
    else if (pIndexBuffer_)
    {
        if (pIndexBuffer_->GetSize() != sizeof T)
        {
            throw ENGINE_EXCEPTION("The index buffer data does not conform to the size of the index buffer");
        }
    }
    else
    {
        throw ENGINE_EXCEPTION(
            "Can not set the index buffer data without setting descriptors first or an existing index buffer");
    }

    size_t count = indexCount_ ? indexCount_.value() : pIndexBuffer_->GetCount();
    if (data->size() < count)
    {
        throw ENGINE_EXCEPTION("The data passed contains fewer than the required number of indexes");
    }

    indexData_ = std::move(data->data());
    didIndexBufferDataChange_ = true;
}
