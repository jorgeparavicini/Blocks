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
        worldActor->AddComponent<Blocks::World>(game->MainCamera().GetTransform());
        //world = worldActor->AddComponent<Blocks::World>(game.MainCamera().GetTransform());
        //world->SetPlayerTransform(std::shared_ptr<BlocksEngine::Transform>{&game.MainCamera().GetTransform()});

        /*BlocksEngine::Actor& chunkActor = game.AddActor();
        Blocks::World world{};

        Blocks::Chunk& chunk = chunkActor.AddComponent<Blocks::Chunk>(world);
        chunk.RegenerateMesh();

        BlocksEngine::Actor& chunkActor2 = game.AddActor();
        Blocks::Chunk& chunk2 = chunkActor2.AddComponent<Blocks::Chunk>(world, BlocksEngine::Vector2{16, 0});
        chunk2.RegenerateMesh();*/
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
