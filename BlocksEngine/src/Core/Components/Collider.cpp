#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Collider.h"

#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Dispatch/DispatchQueue.h"
#include "BlocksEngine/Exceptions/EngineException.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Collider::Collider(std::vector<physx::PxVec3> vertices, std::vector<int32_t> indices)
    : vertices_{std::move(vertices)},
      indices_{std::move(indices)}
{
}

void Collider::Start()
{
    GetTransform()->AddSignalOnMove([this](const Vector3<float>& pos)
    {
        if (actor_)
        {
            actor_->setGlobalPose({pos, actor_->getGlobalPose().q});
        }
    });
    GetTransform()->AddSignalOnRotate([this](const Quaternion& orientation)
    {
        if (actor_)
        {
            actor_->setGlobalPose({actor_->getGlobalPose().p, orientation});
        }
    });

    const auto workItem = std::make_shared<DispatchWorkItem>([this]
    {
        physx::PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = vertices_.size();
        meshDesc.points.stride = sizeof physx::PxVec3;
        meshDesc.points.data = vertices_.data();

        meshDesc.triangles.count = indices_.size() / 3;
        meshDesc.triangles.stride = 3 * sizeof int32_t;
        meshDesc.triangles.data = indices_.data();

        physx::PxDefaultMemoryOutputStream writeBuffer;
        if (!GetGame()->GetPhysics().GetCooking().cookTriangleMesh(meshDesc, writeBuffer))
        {
            throw ENGINE_EXCEPTION("Could not cook delicacies");
        }

        physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
        physx::PxTriangleMeshGeometry triGeom;
        auto& physics = GetGame()->GetPhysics();
        triGeom.triangleMesh = physics.GetPhysics().createTriangleMesh(readBuffer);

        const auto transform = physx::PxTransform{GetTransform()->GetPosition(), GetTransform()->GetOrientation()};
        actor_ = GetGame()->GetPhysics().GetPhysics().createRigidStatic(transform);
        GetGame()->GetPhysics().GetScene().addActor(*actor_);

        const auto shape = physics.GetPhysics().createShape(triGeom, physics.DefaultMaterial());

        actor_->attachShape(*shape);
        shape->release();
    });

    DispatchQueue::Background()->Async(workItem);
}
