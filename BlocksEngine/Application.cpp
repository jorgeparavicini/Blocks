#include "pch.h"
#include "Application.h"

BlocksEngine::Application::Application()
    : window_{std::make_unique<Window>()}
{
}

bool BlocksEngine::Application::GetCloseOnWindowsDestroyed() const noexcept
{
    return closeOnWindowsDestroyed_;
}

void BlocksEngine::Application::SetCloseOnWindowsDestroyed(const bool value) noexcept
{
    closeOnWindowsDestroyed_ = value;
}

int BlocksEngine::Application::MainLoop()
{
    while (true)
    {
        if (shutdownForced_) return ForceShutdown();
    }
}

void BlocksEngine::Application::RequestShutdown() noexcept
{
    shutdownRequested_ = true;
}

void BlocksEngine::Application::RequestImmediateShutdown() noexcept
{
    shutdownForced_ = true;
}

int BlocksEngine::Application::ForceShutdown()
{
    // TODO: Implement
    return 0;
}
