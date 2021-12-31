#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/CharacterController.h"

#include <boost/bind.hpp>

#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

CharacterController::CharacterController()
{
}

CharacterController::~CharacterController()
{
    if (controller_)
    {
        controller_->release();
    }
}

void CharacterController::Start()
{
    physx::PxControllerManager* manager = PxCreateControllerManager(GetGame()->GetPhysics().GetScene());
    physx::PxBoxControllerDesc desc;
    desc.halfHeight = 1.0f;
    desc.halfForwardExtent = 0.45f;
    desc.halfSideExtent = 0.45f;
    desc.contactOffset = 0.01f;

    // Capsule
    /* desc.height = 2.0f;
    desc.radius = 0.5f;
    desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;*/
    desc.stepOffset = 0.1f;
    desc.material = &GetGame()->GetPhysics().DefaultMaterial();
    const auto pos = GetTransform()->GetPosition();
    desc.position = {pos.x, pos.y, pos.z};

    controller_ = manager->createController(desc);

    // TODO: This also releases the controller. We should make this static
    //manager->release();

    // TODO: find out how to bind this correctly so we can use OnMove
    GetTransform()->AddSignalOnMove([this](const Vector3<float>& newPos)
    {
        controller_->setPosition({newPos.x, newPos.y, newPos.z});
    });
}

void CharacterController::Move(const Vector3<float>& motion) const
{
    assert(controller_);

    controller_->move(motion, 0.0f, GetGame()->Time().DeltaTime(), {});
    const auto newPos = static_cast<Vector3<float>>(controller_->getPosition());
    GetTransform()->SetPosition(newPos);
}

void CharacterController::OnMove(const Vector3<>& newPos) const
{
    controller_->setPosition({newPos.x, newPos.y, newPos.z});
}
