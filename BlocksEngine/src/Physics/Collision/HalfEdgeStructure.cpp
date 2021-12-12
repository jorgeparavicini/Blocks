#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/HalfEdgeStructure.h"

#include <robin_hood.h>

using namespace BlocksEngine;

HalfEdgeStructure::Vertex::Vertex(const unsigned int vertexCoordsIndex)
    : vertexPointIndex{vertexCoordsIndex}, edgeIndex{0}
{
}

HalfEdgeStructure::Face::Face()
    : edgeIndex{0}, faceVertices{}
{
}

HalfEdgeStructure::Face::Face(std::vector<unsigned> vertices)
    : edgeIndex{0}, faceVertices{std::move(vertices)}
{
}

HalfEdgeStructure::HalfEdgeStructure(const unsigned int facesCapacity,
                                     const unsigned int verticesCapacity,
                                     const unsigned int edgesCapacity)
    : vertices_(verticesCapacity),
      edges_(edgesCapacity),
      faces_(facesCapacity)
{
}

void HalfEdgeStructure::Init()
{
    robin_hood::unordered_map<VertexPair, Edge> edges{};
    robin_hood::unordered_map<VertexPair, VertexPair> nextEdges{};
    robin_hood::unordered_map<VertexPair, unsigned int> mapEdgeToStartVertex{};
    robin_hood::unordered_map<VertexPair, unsigned int> mapEdgeToIndex{};
    robin_hood::unordered_map<unsigned int, VertexPair> mapEdgeIndexToKey{};
    robin_hood::unordered_map<unsigned int, VertexPair> mapFaceIndexToEdgeKey{};

    std::vector<VertexPair> currentFaceEdges(faces_[0].faceVertices.size());

    for (int f = 0; f < faces_.size(); ++f)
    {
        Face face = faces_[f];

        VertexPair firstEdgeKey{0, 0};

        for (int v = 0; v < face.faceVertices.size(); ++v)
        {
            int v1Index = face.faceVertices[v];
            int v2Index = face.faceVertices[v == face.faceVertices.size() - 1 ? 0 : v + 1];

            const auto pairV1V2 = VertexPair{v1Index, v2Index};

            Edge edge;
            edge.faceIndex = f;
            edge.vertexIndex = v1Index;
            if (v == 0)
            {
                firstEdgeKey = pairV1V2;
            }
            else if (v >= 1)
            {
                nextEdges[currentFaceEdges[currentFaceEdges.size() - 1]] = pairV1V2;
            }
            if (v == face.faceVertices.size() - 1)
            {
                nextEdges[pairV1V2] = firstEdgeKey;
            }

            edges[pairV1V2] = edge;

            const VertexPair pairV2V1{v2Index, v1Index};

            mapEdgeToStartVertex[pairV1V2] = v1Index;
            mapEdgeToStartVertex[pairV2V1] = v2Index;

            mapFaceIndexToEdgeKey[f] = pairV1V2;

            auto itEdge = edges.find(pairV2V1);
            if (itEdge != edges.end())
            {
                const unsigned int edgeIndex = edges_.size();

                itEdge->second.twinEdgeIndex = edgeIndex + 1;
                edge.twinEdgeIndex = edgeIndex;

                mapEdgeIndexToKey[edgeIndex] = pairV2V1;
                mapEdgeIndexToKey[edgeIndex + 1] = pairV1V2;

                vertices_[v1Index].edgeIndex = edgeIndex + 1;
                vertices_[v2Index].edgeIndex = edgeIndex;

                mapEdgeToIndex[pairV1V2] = edgeIndex + 1;
                mapEdgeToIndex[pairV2V1] = edgeIndex;

                edges_.push_back(itEdge->second);
                edges_.push_back(edge);
            }

            currentFaceEdges.push_back(pairV1V2);
        }

        currentFaceEdges.clear();
    }

    // Set next edges
    for (int i = 0; i < edges_.size(); ++i)
    {
        edges_[i].nextEdgeIndex = mapEdgeToIndex[nextEdges[mapEdgeIndexToKey[i]]];
    }

    // Set face edge
    for (int f = 0; f < faces_.size(); ++f)
    {
        faces_[f].edgeIndex = mapEdgeToIndex[mapFaceIndexToEdgeKey[f]];
    }
}
