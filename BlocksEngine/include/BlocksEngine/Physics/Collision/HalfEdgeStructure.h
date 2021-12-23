// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: HalfEdgeStructure.h

#pragma once

namespace BlocksEngine
{
    class HalfEdgeStructure;
}

// TODO: Documentation
// TODO: License
// TODO: Convert unsigned stuff to int

class BlocksEngine::HalfEdgeStructure
{
public:
    //------------------------------------------------------------------------------
    // Types
    //------------------------------------------------------------------------------

    using VertexPair = std::pair<unsigned int, unsigned int>;

    struct Vertex
    {
        Vertex(unsigned int vertexCoordsIndex);

        unsigned int vertexPointIndex;
        unsigned int edgeIndex;
    };

    struct Edge
    {
        unsigned int vertexIndex;
        unsigned int twinEdgeIndex;
        unsigned int faceIndex;
        unsigned int nextEdgeIndex;
    };

    struct Face
    {
        Face();
        Face(std::vector<unsigned int> vertices);


        unsigned int edgeIndex;
        std::vector<unsigned int> faceVertices;
    };


    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    HalfEdgeStructure(unsigned int facesCapacity, unsigned int verticesCapacity, unsigned int edgesCapacity);

    HalfEdgeStructure(const HalfEdgeStructure&) = delete;
    HalfEdgeStructure& operator=(const HalfEdgeStructure&) = delete;

    HalfEdgeStructure(const HalfEdgeStructure&&) = delete;
    HalfEdgeStructure& operator=(const HalfEdgeStructure&&) = delete;

    ~HalfEdgeStructure() = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void Init();

    unsigned int AddVertex(unsigned int vertexPointIndex);

    void AddFace(std::vector<unsigned int> faceVertices);

    [[nodiscard]] unsigned int GetFaceCount() const;

    [[nodiscard]] unsigned int GetHalfEdgeCount() const;

    [[nodiscard]] unsigned int GetVertexCount() const;

    [[nodiscard]] const Face& GetFace(unsigned int index) const;

    [[nodiscard]] const Edge& GetHalfEdge(unsigned int index) const;

    [[nodiscard]] const Vertex& GetVertex(unsigned int index) const;

private:
    std::vector<Vertex> vertices_;
    std::vector<Edge> edges_;
    std::vector<Face> faces_;
};

inline unsigned BlocksEngine::HalfEdgeStructure::AddVertex(const unsigned int vertexPointIndex)
{
    const Vertex vertex{vertexPointIndex};
    vertices_.push_back(vertex);
    return vertices_.size() - 1;
}

inline void BlocksEngine::HalfEdgeStructure::AddFace(std::vector<unsigned> faceVertices)
{
    const Face face{std::move(faceVertices)};
    faces_.push_back(face);
}

inline unsigned BlocksEngine::HalfEdgeStructure::GetFaceCount() const
{
    return static_cast<unsigned int>(faces_.size());
}

inline unsigned BlocksEngine::HalfEdgeStructure::GetHalfEdgeCount() const
{
    return static_cast<unsigned int>(edges_.size());
}

inline unsigned BlocksEngine::HalfEdgeStructure::GetVertexCount() const
{
    return static_cast<unsigned int>(vertices_.size());
}

inline const BlocksEngine::HalfEdgeStructure::Face& BlocksEngine::HalfEdgeStructure::GetFace(
    const unsigned int index) const
{
    assert(index < faces_.size());
    return faces_[index];
}

inline const BlocksEngine::HalfEdgeStructure::Edge& BlocksEngine::HalfEdgeStructure::GetHalfEdge(
    const unsigned int index) const
{
    assert(index < edges_.size());
    return edges_[index];
}

inline const BlocksEngine::HalfEdgeStructure::Vertex& BlocksEngine::HalfEdgeStructure::GetVertex(
    const unsigned int index) const
{
    assert(index < vertices_.size());
    return vertices_[index];
}
