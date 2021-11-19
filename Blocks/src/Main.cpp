#include "Blocks/pch.h"

#include <Windows.h>

#include "Blocks/World.h"
#include "BlocksEngine/Exception.h"
#include "BlocksEngine/Game.h"
#include "BlocksEngine/Renderer.h"


int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    auto game = BlocksEngine::Game::CreateGame();

    game->AddSignalGameStart([&game]
    {
        auto worldActor = game->AddActor(L"World");
        worldActor->AddComponent<Blocks::World>(game->MainCamera().GetTransform(), 16);
    });

    try
    {
        return game->Start();
    }
    catch (const BlocksEngine::Exception& e)
    {
        MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBoxA(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}
