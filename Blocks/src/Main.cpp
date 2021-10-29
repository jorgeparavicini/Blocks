#include "Blocks/pch.h"

#include <Windows.h>
#include <boost/signals2.hpp>
#include <boost/functional/hash.hpp>

#include "Blocks/Block.h"
#include "Blocks/Chunk.h"
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

    auto game = BlocksEngine::Game{};

    game.AddSignalGameStart([&game]
    {
        BlocksEngine::Actor& chunkActor = game.AddActor();
        Blocks::Chunk& chunk = chunkActor.AddComponent<Blocks::Chunk>(BlocksEngine::Vector2{0, 0});
        chunk.RegenerateMesh();
    });

    try
    {
        return game.Start();
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
