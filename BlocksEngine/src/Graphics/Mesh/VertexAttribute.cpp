#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/VertexAttribute.h"

#include "BlocksEngine/Exceptions/EngineException.h"

using namespace BlocksEngine;


VertexAttributeDescriptor::VertexAttributeDescriptor(const VertexAttribute attribute,
                                                     const VertexAttributeFormat format,
                                                     const int dimension)
    : VertexAttributeDescriptor{attribute, attributeToSemantic_.at(attribute), format, dimension}
{
}

VertexAttributeDescriptor::VertexAttributeDescriptor(std::string semantic,
                                                     const VertexAttributeFormat format,
                                                     const int dimension)
    : VertexAttributeDescriptor{VertexAttribute::Custom, std::move(semantic), format, dimension}
{
}

VertexAttributeDescriptor& VertexAttributeDescriptor::operator=(VertexAttributeDescriptor descriptor)
{
    if (this != &descriptor)
    {
        attribute_ = descriptor.attribute_;
        semantic_ = descriptor.semantic_;
        format_ = descriptor.format_;
        dimension_ = descriptor.dimension_;
    }

    return *this;
}

VertexAttributeDescriptor::VertexAttributeDescriptor(const VertexAttribute attribute,
                                                     std::string semantic,
                                                     const VertexAttributeFormat format,
                                                     const int dimension)
    : attribute_{attribute},
      semantic_{std::move(semantic)},
      format_{format},
      dimension_{dimension}
{
}

size_t VertexAttributeDescriptor::GetFormatSize() const noexcept
{
    switch (format_)
    {
    case VertexAttributeFormat::Float16:
        return 2;

    case VertexAttributeFormat::Float32:
        return 4;

    case VertexAttributeFormat::UNorm8:
        return 1;

    case VertexAttributeFormat::UNorm16:
        return 2;

    case VertexAttributeFormat::SNorm8:
        return 1;

    case VertexAttributeFormat::SNorm16:
        return 2;

    case VertexAttributeFormat::UInt8:
        return 1;

    case VertexAttributeFormat::UInt16:
        return 2;

    case VertexAttributeFormat::UInt32:
        return 4;

    case VertexAttributeFormat::SInt8:
        return 1;

    case VertexAttributeFormat::SInt16:
        return 2;

    case VertexAttributeFormat::SInt32:
        return 4;

    default:
        throw ENGINE_EXCEPTION("Invalid Vertex Attribute Format");
    }
}

VertexAttributeDescriptor::operator D3D11_INPUT_ELEMENT_DESC() const
{
    // TODO: Implement
}
