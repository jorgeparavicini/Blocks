#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConcaveMeshShape.h"

#include "BlocksEngine/Physics/Collision/RaycastInfo.h"

using namespace BlocksEngine;

ConvexTriangleAABBOverlapCallback::ConvexTriangleAABBOverlapCallback(TriangleCallback& triangleCallback,
                                                                     const ConcaveMeshShape& concaveMeshShape,
                                                                     const DynamicAABBTree& dynamicAABBTree)
    : triangleTestCallback_{triangleCallback},
      concaveMeshShape_{concaveMeshShape},
      dynamicAABBTree_{dynamicAABBTree}
{
}

ConcaveMeshRaycastCallback::ConcaveMeshRaycastCallback(const DynamicAABBTree& dynamicAABBTree,
                                                       const ConcaveMeshShape& concaveMeshShape,
                                                       std::shared_ptr<Collider> collider,
                                                       RaycastInfo& raycastInfo, const Ray& ray,
                                                       const Vector3<>& meshScale)
    : hitAABBNodes_{},
      dynamicAABBTree_{dynamicAABBTree},
      concaveMeshShape_{concaveMeshShape},
      collider_{std::move(collider)},
      raycastInfo_{raycastInfo},
      ray_{ray},
      isHit_{false},
      meshScale_{meshScale}
{
}

float ConcaveMeshRaycastCallback::RaycastBroadPhaseShape(const int32_t nodeId, const Ray& ray)
{
    hitAABBNodes_.push_back(nodeId);

    return ray.maxFraction;
}

void ConcaveMeshRaycastCallback::RaycastTriangles()
{
    float minHitFraction = ray_.maxFraction;

    for (const int node : hitAABBNodes_)
    {
        const int32_t* data = dynamicAABBTree_.GetNodeDataInt(node);
        const std::array<Vector3<float>, 3> trianglePoints = concaveMeshShape_.GetTriangleVertices(data[1]);
        const std::array<Vector3<float>, 3> triangleNormals = concaveMeshShape_.GetTriangleNormals(data[1]);

        TriangleShape triangleShape{trianglePoints, triangleNormals, concaveMeshShape_.ComputeTriangleShapeId(data[1])};
        triangleShape.SetRaycastTestType(concaveMeshShape_.GetRaycastTestType());

        RaycastInfo raycastInfo;
        const bool isHit = triangleShape.Raycast(ray_, raycastInfo, collider_);

        if (isHit && raycastInfo.hitFraction <= minHitFraction)
        {
            assert(raycastInfo.hitFraction >= 0.0f);
            raycastInfo_.body = raycastInfo.body;
            raycastInfo_.collider = raycastInfo.collider;
            raycastInfo_.hitFraction = raycastInfo.hitFraction;
            raycastInfo_.worldPoint = raycastInfo.worldPoint * meshScale_;
            raycastInfo_.worldNormal = raycastInfo.worldNormal;
            raycastInfo_.meshSubpart = data[0];
            raycastInfo_.triangleIndex = data[1];

            minHitFraction = raycastInfo.hitFraction;
            isHit_ = true;
        }
    }
}

std::string ConcaveMeshShape::ToString() const
{
    std::stringstream ss;
    ss << "ConcaveMeshShape{" << "\n";

    ss << "Vertex Count = " << mesh_->GetVertexCount() << "\n";
    ss << "Triangle Count = " << mesh_->GetTriangleCount() << "\n";

    ss << "Vertices = [";
    for (int v = 0; v < mesh_->GetVertexCount(); ++v)
    {
        auto pos = mesh_->GetVertexValue<float>(v, VertexAttribute::Position);
        ss << "(";
        for (const float p : pos)
        {
            ss << std::to_string(p) << ", ";
        }
        ss << ")";
    }

    // TODO: add normals and triangles

    return ss.str();
}

ConcaveMeshShape::ConcaveMeshShape(const std::shared_ptr<Mesh> mesh, const Vector3<>& scaling)
    : ConcaveShape{CollisionShapeName::TriangleMesh, scaling}
{
    mesh_ = mesh;
    raycastTestType_ = TriangleRaycastSide::Front;

    // Insert all the triangles into the dynamic AABB tree
    InitBHVTree();
    ComputeNormals();
}

void ConcaveMeshShape::InitBHVTree()
{
    for (int index = 0; index < mesh_->GetTriangleCount(); ++index)
    {
        std::array<Vector3<float>, 3> points = mesh_->GetTriangle(index);
        AABB aabb = AABB::CreateFromTriangle(points);
        // Currently we don't support submeshes, hence we set data1 to 0
        dynamicAABBTree_.AddObject(aabb, 0, index);
    }
}

void ConcaveMeshShape::ComputeNormals()
{
    std::vector<Vector3<float>> normals(mesh_->GetVertexCount());

    for (int i = 0; i < mesh_->GetTriangleCount(); ++i)
    {
        const auto indices = mesh_->GetTriangleIndices(i);
        const auto vertices = mesh_->GetTriangle(i);

        const float edgesLength[3] = {
            (vertices[1] - vertices[0]).Length(),
            (vertices[2] - vertices[1]).Length(),
            (vertices[0] - vertices[2]).Length()
        };

        for (int j = 0; j < 3; ++j)
        {
            const int previousVertex = j == 0 ? 2 : j - 1;
            const int nextVertex = j == 2 ? 0 : j + 1;

            Vector3<float> a = vertices[nextVertex] - vertices[j];
            Vector3<float> b = vertices[previousVertex] - vertices[j];

            Vector3<float> crossProduct = a.Cross(b);
            float sinA = crossProduct.Length() / (edgesLength[previousVertex] * edgesLength[j]);
            sinA = std::min(std::max(sinA, 0.0f), 1.0f);
            const float arcSinA = std::asin(sinA);
            assert(arcSinA >= 0.0f);
            const Vector3<float> normalComponent = arcSinA * crossProduct;

            normals[static_cast<size_t>(indices[j]) * 3] += normalComponent;
        }
    }

    for (int i = 0; i < mesh_->GetVertexCount(); i += 3)
    {
        Vector3 normal{normals[i], normals[i + 1], normals[i + 2]};
        normal.Normalize();

        normals[i] = normal;
    }

    computedVerticesNormals_ = std::move(normals);
}

std::array<Vector3<>, 3> ConcaveMeshShape::GetTriangleVertices(const int index) const
{
    std::array<Vector3<float>, 3> result = mesh_->GetTriangle(index);

    result[0].x *= scale_.x;
    result[0].y *= scale_.y;
    result[0].z *= scale_.z;

    result[1].x *= scale_.x;
    result[1].y *= scale_.y;
    result[1].z *= scale_.z;

    result[2].x *= scale_.x;
    result[2].y *= scale_.y;
    result[2].z *= scale_.z;

    return result;
}

std::array<Vector3<>, 3> ConcaveMeshShape::GetTriangleNormals(const int index) const
{
    std::array<Vector3<float>, 3> result;
    result[0] = computedVerticesNormals_[index];
    result[1] = computedVerticesNormals_[static_cast<size_t>(index) + 1];
    result[2] = computedVerticesNormals_[static_cast<size_t>(index) + 2];
    return result;
}

std::array<int, 3> ConcaveMeshShape::GetTriangleIndices(const int index) const
{
    return mesh_->GetTriangleIndices(index);
}

int ConcaveMeshShape::GetTriangleCount() const
{
    return mesh_->GetTriangleCount();
}

void ConcaveMeshShape::ComputeOverlappingTriangles(const AABB& localAABB, std::vector<Vector3<>>& triangleVertices,
                                                   std::vector<Vector3<>>& triangleVertexNormals,
                                                   std::vector<int>& shapeIds) const
{
    AABB aabb{localAABB};
    aabb.Scale({1.0f / scale_.x, 1.0f / scale_.y, 1.0f / scale_.z});

    std::vector<int> overlappingNodes{};
    dynamicAABBTree_.ReportAllShapesOverlappingWithAABB(aabb, overlappingNodes);

    const int overlappingNodeCount = overlappingNodes.size();

    for (int i = 0; i < overlappingNodeCount; ++i)
    {
        const int nodeId = overlappingNodes[i];
        const int32_t* data = dynamicAABBTree_.GetNodeDataInt(nodeId);

        auto vertices = GetTriangleVertices(data[1]);
        for (auto& vertex : vertices)
        {
            triangleVertices.push_back(std::move(vertex));
        }

        auto normals = GetTriangleNormals(data[1]);
        for (auto& normal : normals)
        {
            triangleVertexNormals.push_back(std::move(normal));
        }

        shapeIds.push_back(ComputeTriangleShapeId(data[1]));
    }
}

bool ConcaveMeshShape::Raycast(const Ray& ray, RaycastInfo& raycastInfo, std::shared_ptr<Collider> collider) const
{
    const Vector3 inverseScale{1.0f / scale_.x, 1.0f / scale_.y, 1.0f / scale_.z};
    const Ray scaledRay{ray.point1 * inverseScale, ray.point2 * inverseScale, ray.maxFraction};

    ConcaveMeshRaycastCallback raycastCallback{
        dynamicAABBTree_, *this, std::move(collider), raycastInfo, scaledRay, scale_
    };

    dynamicAABBTree_.Raycast(scaledRay, raycastCallback);
    raycastCallback.RaycastTriangles();
    return raycastCallback.IsHit();
}

int ConcaveMeshShape::ComputeTriangleShapeId(const int index) const
{
    // REMARK: If we allow submeshes then we have to calculate the id.
    return index;
}
