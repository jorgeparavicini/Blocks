#include "BlocksEngine/pch.h"
#include "BlocksEngine/Game.h"

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Camera.h"
#include "BlocksEngine/Renderer.h"

using namespace BlocksEngine;


Game::Game()
    : Game{std::make_unique<WindowOptions>()}
{
}

Game::Game(std::unique_ptr<WindowOptions> options)
    : pWindow_{std::make_unique<Window>(std::move(options))}
{
    Actor& cameraActor = AddActor();
    Camera& camera = cameraActor.AddComponent<Camera>();
    camera.GetTransform().SetPosition(Vector3(0, 0, 20));
    SetActiveCamera(camera);

    Actor& blockActor = AddActor();
    blockActor.AddComponent<Renderer>();
}

int Game::MainLoop() const
{
    while (true)
    {
        if (shutdownForced_) return 0;

        if (const auto eCode = pWindow_->ProcessMessages())
        {
            return *eCode;
        }

        if (const auto eCode = ProcessApplicationMessages())
        {
            return *eCode;
        }

        Tick();
    }
}

Window& Game::GetWindow() const noexcept
{
    return *pWindow_;
}

const Graphics& Game::GetGraphics() const noexcept
{
    return GetWindow().Gfx();
}

void Game::Exit() noexcept
{
    PostQuitMessage(0);
}

void Game::ForceExit() noexcept
{
    shutdownForced_ = true;
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

Actor& Game::AddActor()
{
    auto actor = std::make_unique<Actor>(*this);
    Actor& a = *actor;
    pActors_.push_back(std::move(actor));
    return a;
}

void Game::Tick() const
{
    // TODO: Tick & Update
    Update();
    Render();
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
    pWindow_->Clear();

    for (const auto& pActor : pActors_)
    {
        pActor->Render();
    }

    pWindow_->Present();
}


std::optional<int> Game::ProcessApplicationMessages() noexcept
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return static_cast<int>(msg.wParam);
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}
