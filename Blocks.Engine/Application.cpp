#include "stdafx.h"
#include "Application.h"

bool BlocksEngine::Application::GetCloseOnWindowsDestroyed() const noexcept
{
    return closeOnWindowsDestroyed_;
}

void BlocksEngine::Application::SetCloseOnWindowsDestroyed(const bool value) noexcept
{
    closeOnWindowsDestroyed_ = value;
}

void BlocksEngine::Application::RegisterWindow(std::unique_ptr<Window> window) noexcept
{
    windows_.push_back(std::move(window));
}
