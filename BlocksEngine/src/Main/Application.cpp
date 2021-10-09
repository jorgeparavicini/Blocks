#include "BlocksEngine/pch.h"
#include "BlocksEngine/Application.h"

#include "BlocksEngine/SolidColor.h"

BlocksEngine::Application::Application() : Application{std::make_unique<WindowOptions>()}
{
}

BlocksEngine::Application::Application(std::unique_ptr<WindowOptions> options)
    : pWindow_{std::make_unique<Window>(std::move(options))},
      pGame_{std::make_shared<Game>(*this)}
{
}

int BlocksEngine::Application::MainLoop() const
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

const BlocksEngine::Window& BlocksEngine::Application::GetWindow() const noexcept
{
    return *pWindow_;
}

const BlocksEngine::Graphics& BlocksEngine::Application::GetGraphics() const noexcept
{
    return GetWindow().Gfx();
}

void BlocksEngine::Application::Exit() noexcept
{
    PostQuitMessage(0);
}

void BlocksEngine::Application::ForceExit() noexcept
{
    shutdownForced_ = true;
}

void BlocksEngine::Application::Tick() const
{
    // TODO: Tick & Update
    Render();
}

void BlocksEngine::Application::Update() const
{
    pGame_->Update();
}

void BlocksEngine::Application::Render() const
{
    pWindow_->Clear();

    pGame_->Render();

    pWindow_->Present();
}

std::optional<int> BlocksEngine::Application::ProcessApplicationMessages() noexcept
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
