#include "Blocks/pch.h"

#include <chrono>
#include <thread>
#include <Blocks/Player/PlayerMovement.h>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "Blocks/Player/PlayerDebugs.h"
#include "Blocks/World/World.h"
#include "BlocksEngine/Core/Actor.h"
#include "BlocksEngine/Core/Components/CharacterController.h"
#include "BlocksEngine/Exceptions/Exception.h"
#include "BlocksEngine/Main/Game.h"

void SetupLogging()
{
    namespace Logging = boost::log;
    namespace Keywords = boost::log::keywords;

    Logging::register_simple_formatter_factory<Logging::trivial::severity_level, char>("Severity");
    Logging::add_file_log(
        Keywords::file_name = "blocks.log",
        Keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
    );

    Logging::core::get()->set_filter(
        Logging::trivial::severity >= Logging::trivial::trace
    );

    Logging::add_common_attributes();
}


int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nShowCmd);

    SetupLogging();

    try
    {
        auto game = BlocksEngine::Game::CreateGame();

        game->AddSignalGameStart([&game]
        {
            const auto cameraActor = game->MainCamera().GetActor();
            cameraActor->GetTransform()->SetPosition({0, 30, 0});
            cameraActor->AddComponent<Blocks::PlayerDebugs>();
            auto characterController = cameraActor->AddComponent<BlocksEngine::CharacterController>();
            cameraActor->AddComponent<Blocks::PlayerMovement>(std::move(characterController));

            const auto worldActor = game->AddActor(L"World");
            worldActor->AddComponent<Blocks::World>(game->MainCamera().GetTransform(), 4);
        });
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
