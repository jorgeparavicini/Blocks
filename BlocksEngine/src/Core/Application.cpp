#include "BlocksEngine/pch.h"
#include "BlocksEngine/Application.h"

BlocksEngine::Application::Application(std::unique_ptr<WindowOptions> options)
    : window_{std::move(options)}
{
}

int BlocksEngine::Application::MainLoop() const
{
    while (true)
    {
        if (shutdownForced_) return 0;

        if (const auto eCode = window_.ProcessMessages())
        {
            return *eCode;
        }
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
