// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetGame.h

#pragma once

#include <boost/signals2/signal.hpp>

#include "BlocksEngine/Core/Time.h"
#include "BlocksEngine/Core/Components/Camera.h"
#include "BlocksEngine/Core/Dispatch/BaseDispatchQueue.h"
#include "BlocksEngine/Core/Dispatch/MainDispatchQueue.h"
#include "BlocksEngine/Core/Input/Keyboard.h"
#include "BlocksEngine/Core/Input/Mouse.h"
#include "BlocksEngine/Graphics/Graphics.h"
#include "BlocksEngine/Main/Window.h"
#include "BlocksEngine/Main/WindowOptions.h"

namespace BlocksEngine
{
    class Game;
}

// TODO: The game does not need to be shared as a shared_ptr tbh. It could just be a reference.
// The actors however do need to be shared as shared_ptr.

class BlocksEngine::Game : public std::enable_shared_from_this<Game>
{
public:
    //------------------------------------------------------------------------------
    // Type definitions
    //------------------------------------------------------------------------------

    using GameStartSignal = boost::signals2::signal<void()>;

    //------------------------------------------------------------------------------
    // Destructors | Copy | Move
    //------------------------------------------------------------------------------

    ~Game();

    Game(const Game& game) = delete;
    Game& operator=(const Game& game) = delete;

    Game(const Game&& game) = delete;
    Game& operator=(const Game&& game) = delete;

    static std::shared_ptr<Game> CreateGame(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>())
    {
        // Since a shared_ptr needs to be created to use a game, the constructor is private.
        // And since the constructor is private, make_shared cannot be used
        auto game = std::shared_ptr<Game>(new Game{std::move(options)});
        game->Initialize();
        return game;
    }

    //------------------------------------------------------------------------------
    // TODO: STUFF THAT NEEDS TO BE CLEANED UP (but not now :3)
    //------------------------------------------------------------------------------

    /**
     * \brief Starts the blocking Main Loop. 
     * \return Return code if something went wrong. A return value of 0 indicates no failure.
     */
    [[nodiscard]] int Start();
    [[nodiscard]] Window& Window() const noexcept;
    [[nodiscard]] const Graphics& Graphics() const noexcept;
    [[nodiscard]] Camera& MainCamera() const noexcept;
    [[nodiscard]] bool HasCamera() const noexcept;
    [[nodiscard]] const Keyboard& Keyboard() const noexcept;
    [[nodiscard]] const Mouse& Mouse() const noexcept;
    [[nodiscard]] const Time& Time() const noexcept;
    [[nodiscard]] std::shared_ptr<BaseDispatchQueue> MainDispatchQueue() const noexcept;

    [[nodiscard]] std::thread::id GetMainThreadId() const noexcept;


    static void Exit() noexcept;
    void ForceExit() noexcept;
    void SetActiveCamera(Camera& camera);

    std::shared_ptr<Actor> AddActor();
    std::shared_ptr<Actor> AddActor(std::wstring actorName);

    void DestroyActor(const Actor& actor);

    //------------------------------------------------------------------------------
    // Signals
    //------------------------------------------------------------------------------

    boost::signals2::connection AddSignalGameStart(const GameStartSignal::slot_type& slot) noexcept;

private:
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    explicit Game(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>());

    void Initialize();

    int totalActorCount_{0};
    bool shutdownForced_{false};
    std::unique_ptr<BlocksEngine::Window> pWindow_{};
    std::shared_ptr<BlocksEngine::MainDispatchQueue> pMainDispatch_{};
    void Tick() noexcept;
    void Update();
    void Render() const;
    void Render2D() const;
    static std::optional<int> ProcessApplicationMessages() noexcept;

    std::thread::id mainThreadId_;

    Camera* camera_{nullptr};
    std::vector<std::shared_ptr<Actor>> pActors_{};
    std::queue<std::shared_ptr<Actor>> pNewActorsQueue_{};

    void LoadNewActors();

    // Rendering loop Timer
    BlocksEngine::Time time_;

    // Signals
    GameStartSignal gameStarted_{};

    // Actor states
    std::vector<uint8_t> generations_{};
    std::queue<uint32_t> freeIndices_{};
};
