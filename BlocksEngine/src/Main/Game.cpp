#include "BlocksEngine/pch.h"
#include "BlocksEngine/Main/Game.h"

#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Components/Camera.h"
#include "BlocksEngine/Core/Components/Renderer.h"
#include "BlocksEngine/Exceptions/EngineException.h"
#include "BlocksEngine/Main/EventType.h"

using namespace BlocksEngine;

Game::Game(std::unique_ptr<WindowOptions> options)
    : physics_{std::make_unique<Physics>()},
      pWindow_{std::make_unique<BlocksEngine::Window>(std::move(options))},
      pMainDispatch_{std::make_shared<BlocksEngine::MainDispatchQueue>()},
      mainThreadId_{std::this_thread::get_id()}
{
    // TODO: Catch potential error
    // TODO: If we do this here, only one game could be created.
    const HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        throw ENGINE_EXCEPTION("Failed to initialize COM");
    }
}

void Game::Initialize()
{
    const std::shared_ptr<Actor> cameraActor = AddActor(L"Main Camera");
    const std::shared_ptr<Camera> camera = cameraActor->AddComponent<Camera>();
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
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = true;


    HRESULT hr;
    GFX_THROW_INFO(Graphics().GetDevice().CreateRasterizerState(&rasterizerDesc, &rasterizer));
    Graphics().GetContext().RSSetState(rasterizer.Get());
}


void Game::CreateLogger()
{
    // TODO: We need a logging abstraction. For now this will do, but in the future i don't want to need to call BOOST_*
    namespace Logging = boost::log;
    namespace Keywords = boost::log::keywords;

    Logging::register_simple_formatter_factory<Logging::trivial::severity_level, char>("Severity");
    Logging::add_file_log(
        Keywords::file_name = "engine.log",
        Keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    Logging::core::get()->set_filter(
        Logging::trivial::severity >= Logging::trivial::trace
    );

    Logging::add_common_attributes();
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

        pMainDispatch_->HandleQueue();

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

Physics& Game::GetPhysics()
{
    return *physics_;
}

std::shared_ptr<BaseDispatchQueue> Game::MainDispatchQueue() const noexcept
{
    return pMainDispatch_;
}

std::thread::id Game::GetMainThreadId() const noexcept
{
    return mainThreadId_;
}

const boost::log::sources::logger_mt& Game::Logger()
{
    return logger_;
}

std::shared_ptr<Actor> Game::AddActor()
{
    std::wstring name = L"Actor " + std::to_wstring(totalActorCount_);
    return AddActor(std::move(name));
}

std::shared_ptr<Actor> Game::AddActor(std::wstring actorName)
{
    uint32_t index;

    // If there are already enough free indices to start using them
    if (freeIndices_.size() > Actor::MINIMUM_FREE_INDICES)
    {
        index = freeIndices_.front();
        freeIndices_.pop();
    }
    else
    {
        generations_.push_back(0);
        index = static_cast<uint32_t>(generations_.size()) - 1;

        assert(index < 1u << Actor::INDEX_BITS);
    }

    auto actor = std::shared_ptr<Actor>(new Actor{
        shared_from_this(),
        index,
        static_cast<uint32_t>(generations_[index]),
        std::move(actorName)
    });

    ++totalActorCount_;

    if (index == pActors_.size())
    {
        pActors_.push_back(actor);
        return actor;
    }

    if (index >= pActors_.size())
    {
        pActors_.resize(static_cast<unsigned long long>(index) + 1);
    }

    pActors_[index] = actor;
    return actor;
}

void Game::UpdateEventTypeForActor(const Actor& actor, EventType eventTypes)
{
    // TODO: Replace with map?
    if ((eventTypes & EventType::Update) == EventType::None)
    {
        updateQueue_.erase(actor.GetIndex());
    }

    if ((eventTypes & EventType::Render) == EventType::None)
    {
        renderQueue_.erase(actor.GetIndex());
    }

    if ((eventTypes & EventType::Render2D) == EventType::None)
    {
        render2DQueue_.erase(actor.GetIndex());
    }

    if ((eventTypes & EventType::PhysicsUpdated) == EventType::None)
    {
        physicsUpdatedQueue_.erase(actor.GetIndex());
    }

    if ((eventTypes & EventType::Update) == EventType::Update)
    {
        updateQueue_.insert(actor.GetIndex());
    }

    if ((eventTypes & EventType::Render) == EventType::Render)
    {
        renderQueue_.insert(actor.GetIndex());
    }

    if ((eventTypes & EventType::Render2D) == EventType::Render2D)
    {
        render2DQueue_.insert(actor.GetIndex());
    }

    if ((eventTypes & EventType::PhysicsUpdated) == EventType::PhysicsUpdated)
    {
        physicsUpdatedQueue_.insert(actor.GetIndex());
    }
}

void Game::DestroyActor(std::shared_ptr<Actor> actor)
{
    pDestroyQueue_.push(std::move(actor));
}

boost::signals2::connection Game::AddSignalGameStart(const GameStartSignal::slot_type& slot) noexcept
{
    return gameStarted_.connect(slot);
}

void Game::Tick() noexcept
{
    time_.Tick([&]
    {
        DestroyRequestedActors();
        PhysicsUpdate();
        Update();
    });
    Render();
}

void Game::Update()
{
    int count = 0;
    for (const uint32_t actorId : updateQueue_)
    {
        // TODO: ???????????????? the fuck is this
        if (actorId > 1000)
        {
            int i = 0;
            for (const uint32_t x : updateQueue_)
            {
                OutputDebugString((std::to_wstring(i) + L": " + std::to_wstring(x) + L"\n").c_str());
                ++i;
            }
            OutputDebugStringW((L"Original count: " + std::to_wstring(count)).c_str());
        }
        ++count;
        if (const auto& actor = pActors_[actorId])
        {
            actor->Update();
        }
        else
        {
            // TODO: Logging doesn't work at all

            BOOST_LOG_SEV(logger_, boost::log::trivial::error) << "Tried updating deleted actor. Actor ID: " << actorId;
            abort();
        }
    }
}

void Game::Render() const
{
    pWindow_->Clear();

    for (const int actorId : renderQueue_)
    {
        if (const auto& actor = pActors_[actorId])
        {
            actor->Render();
        }
        else
        {
            abort();
        }
    }

    Render2D();

    pWindow_->Present();
}

void Game::Render2D() const
{
    ID2D1RenderTarget& renderTarget = Graphics().Get2DRenderTarget();
    renderTarget.BeginDraw();

    for (const int actorId : render2DQueue_)
    {
        if (const auto& actor = pActors_[actorId])
        {
            actor->Render2D();
        }
        else
        {
            abort();
        }
    }

    renderTarget.EndDraw();
}

void Game::PhysicsUpdate() const
{
    physics_->Update();
    PhysicsUpdated();
}

void Game::PhysicsUpdated() const
{
    for (const int actorId : physicsUpdatedQueue_)
    {
        if (const auto& actor = pActors_[actorId])
        {
            actor->PhysicsUpdated();
        }
        else
        {
            abort();
        }
    }
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

void Game::DestroyRequestedActors()
{
    while (!pDestroyQueue_.empty())
    {
        const std::shared_ptr<Actor> actor = pDestroyQueue_.front();
        pDestroyQueue_.pop();

        const uint32_t index = actor->GetIndex();

        ++generations_[index];
        freeIndices_.push(index);

        // TODO: Find a better data structure than vector to hold all actors
        // This will cause the vector to relocate.
        // TODO: This really needs to be tested i'm just trying stuff out.
        pActors_[index] = nullptr;

        updateQueue_.erase(index);
        renderQueue_.erase(index);
        render2DQueue_.erase(index);

        // TODO: ????? why
        //AddActor();
    }
}
