// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: VertexAttribute.h

#pragma once

#include <robin_hood.h>
#include <string>

#include "VertexAttribute.h"

namespace BlocksEngine
{
    enum class VertexAttribute;
    enum class VertexAttributeFormat;
    class VertexAttributeDescriptor;
}

enum class BlocksEngine::VertexAttribute
{
    Position,
    Normal,
    Tangent,
    Color,
    TexCoord0,
    TexCoord1,
    TexCoord2,
    TexCoord3,
    TexCoord4,
    TexCoord5,
    TexCoord6,
    TexCoord7,
    BlendWeight,
    BlendIndices,
    Custom
};

enum class BlocksEngine::VertexAttributeFormat
{
    Bool,
    Float32,
    UInt32,
    SInt32
};

class BlocksEngine::VertexAttributeDescriptor
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    VertexAttributeDescriptor(VertexAttribute attribute, VertexAttributeFormat format, int dimension);
    VertexAttributeDescriptor(std::string semantic, VertexAttributeFormat format, int dimension);

    // TODO: Implement rest of these thingies
    VertexAttributeDescriptor& operator=(VertexAttributeDescriptor descriptor);

    [[nodiscard]] VertexAttribute GetAttribute() const noexcept;

    [[nodiscard]] const std::string& GetSemantic() const noexcept;

    [[nodiscard]] VertexAttributeFormat GetFormat() const noexcept;

    [[nodiscard]] int GetDimension() const noexcept;

    [[nodiscard]] size_t GetFormatSize() const noexcept;

    [[nodiscard]] explicit operator D3D11_INPUT_ELEMENT_DESC() const;

private:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    VertexAttributeDescriptor(VertexAttribute attribute, std::string semantic, VertexAttributeFormat format,
                              int dimension);

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    VertexAttribute attribute_;
    std::string semantic_;
    VertexAttributeFormat format_;
    int dimension_;

    const robin_hood::unordered_flat_map<VertexAttribute, const char*> attributeToSemantic_{
        {VertexAttribute::Position, "POSITION"},
        {VertexAttribute::Normal, "NORMAL"},
        {VertexAttribute::Tangent, "TANGENT"},
        {VertexAttribute::Color, "COLOR"},
        {VertexAttribute::TexCoord0, "TEXCOORD0"},
        {VertexAttribute::TexCoord1, "TEXCOORD1"},
        {VertexAttribute::TexCoord2, "TEXCOORD2"},
        {VertexAttribute::TexCoord3, "TEXCOORD3"},
        {VertexAttribute::TexCoord4, "TEXCOORD4"},
        {VertexAttribute::TexCoord5, "TEXCOORD5"},
        {VertexAttribute::TexCoord6, "TEXCOORD6"},
        {VertexAttribute::TexCoord7, "TEXCOORD7"},
        {VertexAttribute::BlendWeight, "BLENDWEIGHT"},
        {VertexAttribute::BlendIndices, "BLENDINDICES"}
    };
};

inline BlocksEngine::VertexAttribute BlocksEngine::VertexAttributeDescriptor::GetAttribute() const noexcept
{
    return attribute_;
}

inline const std::string& BlocksEngine::VertexAttributeDescriptor::GetSemantic() const noexcept
{
    return semantic_;
}

inline BlocksEngine::VertexAttributeFormat BlocksEngine::VertexAttributeDescriptor::GetFormat() const noexcept
{
    return format_;
}

inline int BlocksEngine::VertexAttributeDescriptor::GetDimension() const noexcept
{
    return dimension_;
}
