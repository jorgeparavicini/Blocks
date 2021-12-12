#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Shapes/ConcaveMeshShape.h"

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
                                                       const ConcaveMeshShape& concaveMeshShape, Collider* collider,
                                                       RaycastInfo& raycastInfo, const Ray& ray,
                                                       const Vector3<>& meshScale)
    : hitAABBNodes_{},
      dynamicAABBTree_{dynamicAABBTree},
      concaveMeshShape_{concaveMeshShape},
      collider_{collider},
      raycastInfo_{raycastInfo},
      ray_{ray},
      isHit_{false},
      meshScale_{meshScale}
{
}
