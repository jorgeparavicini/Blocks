#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/TriangleVertexArray.h"

using namespace BlocksEngine;

TriangleVertexArray::TriangleVertexArray(int vertexCount, const void* verticesStart, int verticesStride,
                                         int trianglesCount, const void* indexesStart, int indexesStride,
                                         VertexDataType vertexDataType, IndexDataType indexDataType)
{
}


TriangleVertexArray::TriangleVertexArray(int vertexCount, const void* verticesStart, int verticesStride,
                                         const void* verticesNormalsStart, int verticesNormalsStride,
                                         int trianglesCount, const void* indexesStart, int indexesStride,
                                         VertexDataType vertexDataType, NormalDataType normalDataType,
                                         IndexDataType indexDataType)
{
}
