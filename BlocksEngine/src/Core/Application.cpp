#include "BlocksEngine/pch.h"
#include "BlocksEngine/Application.h"

#include "BlocksEngine/PixelShader.h"
#include "BlocksEngine/VertexShader.h"

BlocksEngine::Application::Application() : Application{std::make_unique<WindowOptions>()}
{
}

BlocksEngine::Application::Application(std::unique_ptr<WindowOptions> options)
    : pWindow_{std::make_unique<Window>(std::move(options))},
      pGame_{std::make_shared<Game>()}
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
        std::shared_ptr<VertexShader> vs = VertexShader::SolidColor(pWindow_->Gfx());
        std::shared_ptr<PixelShader> ps = PixelShader::SolidColor(pWindow_->Gfx());
    }
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
    pWindow_->Render();
}

void BlocksEngine::Application::Update()
{
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
