#include "BlocksEngine/pch.h"
#include "BlocksEngine/Game.h"

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Camera.h"
#include "BlocksEngine/Renderer.h"

using namespace BlocksEngine;

Game::Game(std::unique_ptr<WindowOptions> options)
    : pWindow_{std::make_unique<Window>(std::move(options))}
{
    Actor& cameraActor = AddActor();
    Camera& camera = cameraActor.AddComponent<Camera>();
    camera.GetTransform().SetPosition(Vector3(03, 03, -10));
    const Quaternion rot = Quaternion::Euler(20, -30, 0);
    camera.GetTransform().SetRotation(rot);
    SetActiveCamera(camera);

    Actor& blockActor = AddActor();
    blockActor.AddComponent<Renderer>();

    Actor& block2 = AddActor();
    Vector3 pos = block2.GetTransform().GetPosition();
    pos.y += 1;
    pos.x += 1;
    block2.GetTransform().SetPosition(pos);
    block2.AddComponent<Renderer>();

    Actor& block3 = AddActor();
    Vector3 pos2 = block3.GetTransform().GetPosition();
    pos2.y += 6;
    pos2.x += 4;
    block3.GetTransform().SetPosition(pos2);
    block3.AddComponent<Renderer>();
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

const Keyboard& Game::Keyboard() const noexcept
{
    return GetWindow().GetKeyboard();
}

const Mouse& Game::Mouse() const noexcept
{
    return GetWindow().GetMouse();
}

Actor& Game::AddActor()
{
    std::string name = "Actor " + std::to_string(actorCount_++);
    auto actor = std::make_unique<Actor>(*this, std::move(name));
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
