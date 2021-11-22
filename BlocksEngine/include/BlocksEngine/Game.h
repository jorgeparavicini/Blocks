// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetGame.h

#pragma once

#include "Actor.h"
#include "Camera.h"
#include "MainDispatchQueue.h"
#include "Time.h"
#include "Window.h"
#include "WindowOptions.h"

namespace BlocksEngine
{
    class Game;
}

class BlocksEngine::Game : public std::enable_shared_from_this<Game>
{
public:
    using GameStartSignal = boost::signals2::signal<void()>;

    ~Game();

    static std::shared_ptr<Game> CreateGame(std::unique_ptr<WindowOptions> options = std::make_unique<WindowOptions>())
    {
        // Since a shared_ptr needs to be created to use a game, the constructor is private.
        // And since the constructor is private, make_shared cannot be used
        auto game = std::shared_ptr<Game>{new Game{std::move(options)}};
        game->Initialize();
        return game;
    }

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
    [[nodiscard]] BaseDispatchQueue& MainDispatchQueue() const noexcept;


    static void Exit() noexcept;
    void ForceExit() noexcept;
    void SetActiveCamera(Camera& camera);

    std::shared_ptr<Actor> AddActor();
    std::shared_ptr<Actor> AddActor(std::wstring actorName);

    // Signals
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
    std::unique_ptr<BlocksEngine::MainDispatchQueue> pMainDispatch_{};
    void Tick() noexcept;
    void Update();
    void Render() const;
    void Render2D() const;
    static std::optional<int> ProcessApplicationMessages() noexcept;

    Camera* camera_{nullptr};
    std::vector<std::shared_ptr<Actor>> pActors_{};
    std::queue<std::shared_ptr<Actor>> pNewActorsQueue_{};

    void LoadNewActors();

    // Rendering loop Timer
    BlocksEngine::Time time_;

    // Signals
    GameStartSignal gameStarted_;
};
