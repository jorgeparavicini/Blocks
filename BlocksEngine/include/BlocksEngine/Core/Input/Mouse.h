// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Mouse.h

#pragma once
#include <optional>
#include <queue>

namespace BlocksEngine
{
    class Mouse;
}

class BlocksEngine::Mouse
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            LPress,
            LRelease,
            RPress,
            RRelease,
            WheelUp,
            WheelDown,
            Move,
            Enter,
            Leave,
            Invalid
        };

    private:
        Type type;
        bool isLeftPressed;
        bool isRightPressed;
        int x;
        int y;

    public:
        Event(const Type type, const Mouse& parent) noexcept
            : type{type},
              isLeftPressed{parent.isLeftPressed},
              isRightPressed{parent.isRightPressed},
              x{parent.x},
              y{parent.y}
        {
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }

        [[nodiscard]] Type GetType() const noexcept
        {
            return type;
        }

        [[nodiscard]] std::pair<int, int> GetPos() const noexcept
        {
            return {x, y};
        }

        [[nodiscard]] int GetPosX() const noexcept
        {
            return x;
        }

        [[nodiscard]] int GetPosY() const noexcept
        {
            return y;
        }

        [[nodiscard]] bool IsLeftPressed() const noexcept
        {
            return isLeftPressed;
        }

        [[nodiscard]] bool IsRightPressed() const noexcept
        {
            return isRightPressed;
        }
    };

public:
    Mouse();
    Mouse(const Mouse&) = delete;
    Mouse& operator =(const Mouse&) = delete;
    [[nodiscard]] std::pair<int, int> GetPos() const noexcept;
    [[nodiscard]] int GetPosX() const noexcept;
    [[nodiscard]] int GetPosY() const noexcept;
    [[nodiscard]] bool IsInWindow() const noexcept;
    [[nodiscard]] bool IsLeftPressed() const noexcept;
    [[nodiscard]] bool IsRightPressed() const noexcept;
    [[nodiscard]] std::optional<Event> Read() noexcept;
    [[nodiscard]] bool IsEmpty() const noexcept;
    void Flush() noexcept;

private:
    void OnMouseMove(int newX, int newY) noexcept;
    void OnMouseLeave() noexcept;
    void OnMouseEnter() noexcept;
    void OnLeftPressed(int newX, int newY) noexcept;
    void OnLeftReleased(int newX, int newY) noexcept;
    void OnRightPressed(int newX, int newY) noexcept;
    void OnRightReleased(int newX, int newY) noexcept;
    void OnWheelUp(int newX, int newY) noexcept;
    void OnWheelDown(int newX, int newY) noexcept;
    void TrimBuffer() noexcept;
    void OnWheelDelta(int newX, int newY, int delta) noexcept;

private:
    static constexpr unsigned int BUFFER_SIZE = 16u;
    int x{};
    int y{};
    bool isLeftPressed{};
    bool isRightPressed{};
    bool isInWindow{};
    int wheelDeltaCarry = 0;
    std::queue<Event> buffer;
};
