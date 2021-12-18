// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: TriangleVertexArray.h

#pragma once


#include "BlocksEngine/Core/Math/Math.h"

namespace BlocksEngine
{
    class TriangleVertexArray;
}

class BlocksEngine::TriangleVertexArray
{
public:
    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    enum class VertexDataType
    {
        Float,
        Double
    };

    enum class NormalDataType
    {
        Float,
        Double
    };

    enum class IndexDataType
    {
        Integer,
        Short
    };

    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    TriangleVertexArray(int vertexCount, const void* verticesStart, int verticesStride,
                        int trianglesCount, const void* indexesStart, int indexesStride,
                        VertexDataType vertexDataType, IndexDataType indexDataType);

    TriangleVertexArray(int vertexCount, const void* verticesStart, int verticesStride,
                        const void* verticesNormalsStart, int verticesNormalsStride,
                        int trianglesCount, const void* indexesStart, int indexesStride,
                        VertexDataType vertexDataType, NormalDataType normalDataType,
                        IndexDataType indexDataType);

    TriangleVertexArray(const TriangleVertexArray&) = delete;
    TriangleVertexArray& operator=(const TriangleVertexArray&) = delete;

    TriangleVertexArray(TriangleVertexArray&&) = delete;
    TriangleVertexArray& operator=(TriangleVertexArray&&) = delete;

    ~TriangleVertexArray();


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] VertexDataType GetVertexDataType() const;

    [[nodiscard]] NormalDataType GetVertexNormalDataType() const;

    [[nodiscard]] IndexDataType GetIndexDataType() const;

    [[nodiscard]] int GetVertexCount() const;

    [[nodiscard]] int GetTriangleCount() const;

    [[nodiscard]] int GetVertexStride() const;

    [[nodiscard]] int GetVertexNormalStride() const;

    [[nodiscard]] int GetIndexStride() const;

    [[nodiscard]] const void* GetVerticesStart() const;

    [[nodiscard]] const void* GetVerticesNormalsStart() const;

    [[nodiscard]] const void* GetIndicesStart() const;

    void GetTriangleVertices(int triangleIndex, Vector3<float>* outTriangleVertices) const;

    void GetTriangleVerticesNormals(int triangleIndex, Vector3<float>* outTriangleVerticesNormals) const;

    void GetTriangleVerticesIndices(int triangleIndex, int* outVerticesIndices);

    void GetVertex(int vertexIndex, Vector3<float>* outVertex);

    void GetNormal(int vertexIndex, Vector3<float>* outNormal);


protected:
    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    int vertexCount_;

    const unsigned char* verticesStart_;

    int verticesStride_;

    const unsigned char* verticesNormalsStart_;

    int verticesNormalsStride_;

    int triangleCount_;

    const unsigned char* indicesStart_;

    int indicesStride_;

    VertexDataType vertexDataType_;

    NormalDataType normalDataType_;

    IndexDataType indexDataType_;

    bool areVerticesNormalsProvidedByUser_;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void ComputeVerticesNormals();
};

inline BlocksEngine::TriangleVertexArray::VertexDataType BlocksEngine::TriangleVertexArray::GetVertexDataType() const
{
    return vertexDataType_;
}

inline BlocksEngine::TriangleVertexArray::NormalDataType
BlocksEngine::TriangleVertexArray::GetVertexNormalDataType() const
{
    return normalDataType_;
}

inline BlocksEngine::TriangleVertexArray::IndexDataType BlocksEngine::TriangleVertexArray::GetIndexDataType() const
{
    return indexDataType_;
}

inline int BlocksEngine::TriangleVertexArray::GetVertexCount() const
{
    return vertexCount_;
}

inline int BlocksEngine::TriangleVertexArray::GetTriangleCount() const
{
    return triangleCount_;
}

inline int BlocksEngine::TriangleVertexArray::GetVertexStride() const
{
    return verticesStride_;
}

inline int BlocksEngine::TriangleVertexArray::GetVertexNormalStride() const
{
    return verticesNormalsStride_;
}

inline int BlocksEngine::TriangleVertexArray::GetIndexStride() const
{
    return indicesStride_;
}

inline const void* BlocksEngine::TriangleVertexArray::GetVerticesNormalsStart() const
{
    return verticesNormalsStart_;
}

inline const void* BlocksEngine::TriangleVertexArray::GetIndicesStart() const
{
    return indicesStart_;
}
