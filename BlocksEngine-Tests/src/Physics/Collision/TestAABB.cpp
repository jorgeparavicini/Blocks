#include "pch.h"

#include "BlocksEngine/Core/Math/Vector3.h"
#include "BlocksEngine/Physics/Collision/Shapes/AABB.h"

using namespace BlocksEngine;

TEST(AABBTest, HandlesInitialization)
{
    const AABB aabb1{};
    const AABB aabb2{Vector3<float>{-3.0f, -5.0f, -8.0f}, Vector3<float>{65, -1, 56}};
    const Vector3<float> trianglePoints[] = {
        {-5, 7, 23}, {45, -34, -73}, {-12, 98, 76}
    };

    const AABB aabb3 = AABB::CreateFromTriangle(trianglePoints);

    EXPECT_FLOAT_EQ(aabb1.GetMin().x, 0);
}
