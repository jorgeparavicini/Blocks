#include "Blocks/pch.h"
#include "Blocks/Player/PlayerMovement.h"

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Main/Game.h"

Blocks::PlayerMovement::PlayerMovement(std::weak_ptr<BlocksEngine::CharacterController> controller)
    : controller_{std::move(controller)}
{
}

void Blocks::PlayerMovement::Start()
{
    SetEventTypes(BlocksEngine::EventType::Update);
}

void Blocks::PlayerMovement::Update()
{
    const float deltaTime = GetGame()->Time().DeltaTime();

    velocity_ += static_cast<BlocksEngine::Vector3<float>>(GetGame()->GetPhysics().GetScene().getGravity()) * deltaTime;
    controller_.lock()->Move(velocity_ * deltaTime);
    // TODO: Reset velocity if grounded

    const BlocksEngine::Keyboard& keyboard = GetActor()->GetGame()->Keyboard();

    const auto& playerOrientation = controller_.lock()->GetTransform()->GetOrientation();
    BlocksEngine::Vector3 motion{BlocksEngine::Vector3<float>::Zero};

    if (keyboard.KeyIsPressed('W'))
    {
        motion += playerOrientation * BlocksEngine::Vector3<float>::Forward;
    }
    if (keyboard.KeyIsPressed('S'))
    {
        motion += playerOrientation * BlocksEngine::Vector3<float>::Backward;
    }
    if (keyboard.KeyIsPressed('A'))
    {
        motion += playerOrientation * BlocksEngine::Vector3<float>::Left;
    }
    if (keyboard.KeyIsPressed('D'))
    {
        motion += playerOrientation * BlocksEngine::Vector3<float>::Right;
    }

    controller_.lock()->Move(motion * moveSpeed_ * deltaTime);

    float deltaX{0};
    float deltaY{0};

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

    const auto newPlayerOrientation = BlocksEngine::Quaternion::CreateFromAxisAngle(
        BlocksEngine::Vector3<float>::Up, deltaX * rotationSpeed_ * deltaTime);
    controller_.lock()->GetTransform()->SetOrientation(playerOrientation * newPlayerOrientation);

    const auto newCameraOrientation = BlocksEngine::Quaternion::CreateFromAxisAngle(
        BlocksEngine::Vector3<float>::Right, deltaY * rotationSpeed_ * deltaTime);
    GetTransform()->SetLocalOrientation(GetTransform()->GetLocalOrientation() * newCameraOrientation);
}
