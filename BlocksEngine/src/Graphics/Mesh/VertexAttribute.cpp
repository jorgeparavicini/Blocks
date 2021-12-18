#include "BlocksEngine/pch.h"
#include "BlocksEngine/Graphics/Mesh/VertexAttribute.h"

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
    }
}
