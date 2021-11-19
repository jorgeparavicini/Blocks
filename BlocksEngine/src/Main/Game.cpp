#include "BlocksEngine/pch.h"
#include "BlocksEngine/Game.h"

#include "BlocksEngine/Actor.h"
#include "BlocksEngine/Camera.h"
#include "BlocksEngine/Renderer.h"

using namespace BlocksEngine;

Game::Game(std::unique_ptr<WindowOptions> options)
    : pWindow_{std::make_unique<BlocksEngine::Window>(std::move(options))}
{
    // TODO: Catch potential error
    // TODO: If we do this here, only one game could be created.
    const HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        throw "shit";
    }
}

void Game::Initialize()
{
    const std::shared_ptr<Actor> cameraActor = AddActor(L"Main Camera");
    const std::shared_ptr<Camera> camera = cameraActor->AddComponent<Camera>();
    camera->GetTransform()->SetPosition(Vector3<float>(0, 0, -10));
    const Quaternion rot = Quaternion::Euler(0, 180, 0);
    camera->GetTransform()->SetRotation(rot);
    SetActiveCamera(*camera);

    RECT clientRect;
    GetClientRect(Window().HWnd(), &clientRect);
    auto clientOrigin = POINT{clientRect.left, clientRect.top};
    ClientToScreen(Window().HWnd(), &clientOrigin);
    clientRect.right += clientOrigin.x;
    clientRect.left += clientOrigin.x;
    clientRect.top += clientOrigin.y;
    clientRect.bottom += clientOrigin.y;

    //ClipCursor(&clientRect);


    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;

    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;

    HRESULT hr;
    GFX_THROW_INFO(Graphics().GetDevice().CreateRasterizerState(&rasterizerDesc, &rasterizer));
    Graphics().GetContext().RSSetState(rasterizer.Get());
}

Game::~Game()
{
    CoUninitialize();
}

int Game::Start()
{
    gameStarted_();

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

Window& Game::Window() const noexcept
{
    return *pWindow_;
}

const Graphics& Game::Graphics() const noexcept
{
    return Window().Gfx();
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

Camera& Game::MainCamera() const noexcept
{
    return *camera_;
}

bool Game::HasCamera() const noexcept
{
    return camera_ != nullptr;
}

const Keyboard& Game::Keyboard() const noexcept
{
    return Window().Keyboard();
}

const Mouse& Game::Mouse() const noexcept
{
    return Window().Mouse();
}

const Time& Game::Time() const noexcept
{
    return time_;
}

std::shared_ptr<Actor> Game::AddActor()
{
    std::wstring name = L"GetActor " + std::to_wstring(totalActorCount_++);
    return AddActor(std::move(name));
}

std::shared_ptr<Actor> Game::AddActor(std::wstring actorName)
{
    auto actor = std::make_shared<Actor>(shared_from_this(), std::move(actorName));
    pNewActorsQueue_.push(actor);
    return std::move(actor);
}

boost::signals2::connection Game::AddSignalGameStart(const GameStartSignal::slot_type& slot) noexcept
{
    return gameStarted_.connect(slot);
}

void Game::Tick() noexcept
{
    time_.Tick([&]
    {
        Update();
    });
    Render();
}

void Game::Update()
{
    LoadNewActors();
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

void Game::LoadNewActors()
{
    while (!pNewActorsQueue_.empty())
    {
        pActors_.push_back(std::move(pNewActorsQueue_.front()));
        pNewActorsQueue_.pop();
    }
}
