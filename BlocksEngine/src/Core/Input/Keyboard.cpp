#include "BlocksEngine/pch.h"
#include "BlocksEngine/Keyboard.h"

using namespace BlocksEngine;


bool Keyboard::KeyIsPressed(const unsigned char keycode) const noexcept
{
    return keyStates[keycode];
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
    if (!keyBuffer.empty())
    {
        const Event e = keyBuffer.front();
        keyBuffer.pop();
        return e;
    }
    return {};
}

bool Keyboard::KeyIsEmpty(unsigned char keycode) const noexcept
{
    return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
    keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
    if (!charBuffer.empty())
    {
        const char charCode = charBuffer.front();
        charBuffer.pop();
        return charCode;
    }

    return 0;
}

bool Keyboard::CharIsEmpty() const noexcept
{
    return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
    charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const noexcept
{
    return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(const unsigned char keycode) noexcept
{
    keyStates[keycode] = true;
    keyBuffer.push(Event(Event::Type::Press, keycode));
    TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(const unsigned char keycode) noexcept
{
    keyStates[keycode] = false;
    keyBuffer.push(Event(Event::Type::Release, keycode));
    TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(const char character) noexcept
{
    charBuffer.push(character);
    TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
    keyStates.reset();
}

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > BUFFER_SIZE)
    {
        buffer.pop();
    }
}
