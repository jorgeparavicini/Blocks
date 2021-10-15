// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Keyboard.h

#pragma once
#include <bitset>
#include <optional>
#include <queue>

namespace BlocksEngine
{
    class Keyboard;
}

class BlocksEngine::Keyboard
{
    friend class Window;
public:
    class Event
    {
    public:
        enum class Type
        {
            Press,
            Release,
            Invalid
        };

    private:
        Type type;
        unsigned char code;

    public:
        Event(const Type type, const unsigned char code) noexcept : type{type}, code{code}
        {
        }

        [[nodiscard]] bool IsPressed() const noexcept
        {
            return type == Type::Press;
        }

        [[nodiscard]] bool IsRelease() const noexcept
        {
            return type == Type::Release;
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return type != Type::Invalid;
        }

        [[nodiscard]] unsigned char GetCode() const noexcept
        {
            return code;
        }
    };

    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    // Key events
    [[nodiscard]] bool KeyIsPressed(unsigned char keycode) const noexcept;
    std::optional<Event> ReadKey() noexcept;
    [[nodiscard]] bool KeyIsEmpty(unsigned char keycode) const noexcept;
    void FlushKey() noexcept;

    // Char events
    char ReadChar() noexcept;
    [[nodiscard]] bool CharIsEmpty() const noexcept;
    void FlushChar() noexcept;
    void Flush() noexcept;

    // Auto repeat
    void EnableAutorepeat() noexcept;
    void DisableAutorepeat() noexcept;
    [[nodiscard]] bool IsAutorepeatEnabled() const noexcept;

private:
    void OnKeyPressed(unsigned char keycode) noexcept;
    void OnKeyReleased(unsigned char keycode) noexcept;
    void OnChar(char character) noexcept;
    void ClearState() noexcept;
    template <typename T>
    static void TrimBuffer(std::queue<T>& buffer) noexcept;

    static constexpr unsigned int N_KEYS = 256u;
    static constexpr unsigned int BUFFER_SIZE = 16u;
    bool autorepeatEnabled = false;
    std::bitset<N_KEYS> keyStates{};
    std::queue<Event> keyBuffer;
    std::queue<char> charBuffer;
};
