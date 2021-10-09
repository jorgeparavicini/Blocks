#include "BlocksEngine/pch.h"
#include "BlocksEngine/Game.h"

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Application.h"
#include "BlocksEngine/Camera.h"
#include "BlocksEngine/Renderer.h"

using namespace BlocksEngine;


Game::Game(const Application& application)
    : application_{application}
{
    Actor& cameraActor = AddActor();
    Camera& camera = cameraActor.AddComponent<Camera>();
    camera.GetTransform().SetPosition(Vector3(0, 0, 20));
    SetActiveCamera(camera);

    Actor& blockActor = AddActor();
    blockActor.AddComponent<Renderer>();
}

void Game::SetActiveCamera(Camera& camera)
{
    camera_ = &camera;
}

Camera& Game::GetCamera() const noexcept
{
    return *camera_;
}

bool Game::HasCamera() const noexcept
{
    return camera_ != nullptr;
}

const Application& Game::GetApplication() const noexcept
{
    return application_;
}

const Graphics& Game::GetGraphics() const noexcept
{
    return GetApplication().GetGraphics();
}

Actor& Game::AddActor()
{
    auto actor = std::make_unique<Actor>(*this);
    Actor& a = *actor;
    pActors_.push_back(std::move(actor));
    return a;
}

void Game::Update() const
{
    for (const auto& pActor : pActors_)
    {
        pActor->Update();
    }
}

void Game::Render() const
{
    for (const auto& pActor : pActors_)
    {
        pActor->Render();
    }
}
