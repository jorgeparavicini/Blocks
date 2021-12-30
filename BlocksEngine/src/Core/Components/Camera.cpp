#include "BlocksEngine/pch.h"
#include "BlocksEngine/Core/Components/Camera.h"

#include <boost/bind.hpp>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Transform.h"
#include "BlocksEngine/Core/Input/Keyboard.h"
#include "BlocksEngine/Main/Game.h"

using namespace BlocksEngine;

Matrix Camera::WorldViewProjection() const noexcept
{
    return wvp_;
}

Matrix Camera::WorldView() const noexcept
{
    //const Vector3 position = GetTransform()->GetPosition();
    //const Matrix rotation = Matrix::CreateFromQuaternion(GetTransform()->GetRotation());
    physx::PxShape* shapes[1];
    GetActor()->GetActor().getShapes(shapes, 1, 0);
    const physx::PxTransform shapeTrans = shapes[0]->getLocalPose();
    const auto actorTrans = GetActor()->GetActor().getGlobalPose();
    const auto position = Vector3{actorTrans.p.x, actorTrans.p.y, actorTrans.p.z};
    const auto quat = Quaternion{shapeTrans.q.x, shapeTrans.q.y, shapeTrans.q.z, shapeTrans.q.w};
    const auto rotation = Matrix::CreateFromQuaternion(quat);

    Vector3 target = Vector3<float>::Transform(Vector3<float>::Forward, rotation);
    target.Normalize();

    return Matrix::CreateLookAt(position, position + target, Vector3<float>::Up);
}

Matrix Camera::Projection() const noexcept
{
    return projection_;
}

void Camera::OnWindowResized(const int width, const int height) noexcept
{
    projection_ = std::move(CalculateProjection());
}

void Camera::Update()
{
    worldView_ = WorldView();
    wvp_ = worldView_ * projection_;

    const auto deltaTime = static_cast<float>(GetGame()->Time().DeltaTime());
    const Keyboard& keyboard = GetActor()->GetGame()->Keyboard();
    const auto actor = dynamic_cast<physx::PxRigidDynamic*>(&GetActor()->GetActor());

    //bool isGrounded = physx::
    physx::PxShape* shapes[1];
    actor->getShapes(shapes, 1, 0);
    const auto& o = shapes[0]->getLocalPose().q;
    const auto orientation = Quaternion{o.x, o.y, o.z, o.w};

    physx::PxVec3 movement{physx::PxZero};

    if (keyboard.KeyIsPressed('W'))
    {
        movement += orientation * Vector3<float>::Forward;
    }
    if (keyboard.KeyIsPressed('S'))
    {
        movement += orientation * Vector3<float>::Backward;
    }
    if (keyboard.KeyIsPressed('A'))
    {
        movement += orientation * Vector3<float>::Up;
    }
    if (keyboard.KeyIsPressed('D'))
    {
        movement += orientation * Vector3<float>::Down;
    }

    controller_->move(movement * deltaTime * moveSpeed_, 0.01f, deltaTime, physx::PxControllerFilters{});
    velocity_.y += GetGame()->GetPhysics().GetScene().getGravity().y * deltaTime;
    controller_->move(velocity_ * deltaTime, 0.01f, deltaTime, physx::PxControllerFilters{});

    const Mouse& mouse = GetActor()->GetGame()->Mouse();
    //float deltaX = mouse.GetPosX() - lastX_;
    //float deltaY = mouse.GetPosY() - lastY_;
    float deltaX = 0;
    float deltaY = 0;

    if (keyboard.KeyIsPressed(VK_LEFT))
    {
        deltaX -= keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_RIGHT))
    {
        deltaX += keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_UP))
    {
        deltaY -= keyboardRotationSpeed_;
    }
    if (keyboard.KeyIsPressed(VK_DOWN))
    {
        deltaY += keyboardRotationSpeed_;
    }

    lastX_ = static_cast<float>(mouse.GetPosX());
    lastY_ = static_cast<float>(mouse.GetPosY());

    //rotation_.x = ClampAngle(rotation_.x + deltaY * rotationSpeed_ * deltaTime, -85.0f, 85.0f);
    rotation_.y += deltaX * rotationSpeed_ * deltaTime;

    auto newOrientation = Quaternion::Euler(rotation_);
    auto transform = physx::PxTransform{
        shapes[0]->getLocalPose().p, {newOrientation.x, newOrientation.y, newOrientation.z, newOrientation.w}
    };
    shapes[0]->setLocalPose(transform);
}

void Camera::Start()
{
    SetEventTypes(EventType::Update);

    windowResizedConnection_ = GetGame()->Window().AddSignalWindowResized(
        bind(&Camera::OnWindowResized, this, _1, _2)
    );
    projection_ = CalculateProjection();

    physx::PxControllerManager* manager = PxCreateControllerManager(GetGame()->GetPhysics().GetScene());
    physx::PxCapsuleControllerDesc desc;
    desc.height = 2.0f;
    desc.radius = 0.5f;
    desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
    desc.stepOffset = 0.1f;
    desc.material = &GetGame()->GetPhysics().DefaultMaterial();
    desc.position = physx::PxExtendedVec3{0, 30, 0};

    controller_ = manager->createController(desc);
    GetActor()->actor_ = controller_->getActor();

    /*auto actor = dynamic_cast<physx::PxRigidDynamic*>(&GetActor()->GetActor());
    const physx::PxTransform relativePose{physx::PxQuat{physx::PxHalfPi, physx::PxVec3{0, 0, 1}}};
    physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(
        *actor, physx::PxCapsuleGeometry{0.25f, 0.75f}, GetGame()->GetPhysics().DefaultMaterial());
    shape->setLocalPose(relativePose);
    actor->setRigidDynamicLockFlags(
        physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);*/
}

float Camera::ClampAngle(float angle, const float min, const float max) const
{
    if (angle < -360.0f) angle += 360.0f;
    if (angle > 360.0f) angle -= 360.0f;
    return std::clamp(angle, min, max);
}

DirectX::XMMATRIX Camera::CalculateProjection() const noexcept
{
    return DirectX::XMMatrixPerspectiveFovLH(1.0f, GetGame()->Graphics().AspectRatio(), 0.3f, 1000.0f);
}
