#include "BlocksEngine/pch.h"
#include "BlocksEngine/Mouse.h"

using namespace BlocksEngine;

Mouse::Mouse()
{
    //ShowCursor(FALSE);
}

std::pair<int, int> Mouse::GetPos() const noexcept
{
    return {x, y};
}

int Mouse::GetPosX() const noexcept
{
    return x;
}

int Mouse::GetPosY() const noexcept
{
    return y;
}

bool Mouse::IsInWindow() const noexcept
{
    return isInWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
    return isLeftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
    return isRightPressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
    if (!buffer.empty())
    {
        const Event e = buffer.front();
        buffer.pop();
        return e;
    }

    return {};
}

bool Mouse::IsEmpty() const noexcept
{
    return buffer.empty();
}

void Mouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(const int newX, const int newY) noexcept
{
    x = newX;
    y = newY;

    buffer.push(Event(Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
    isInWindow = false;
    buffer.push(Event(Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
    isInWindow = true;
    buffer.push(Event(Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(const int newX, const int newY) noexcept
{
    isLeftPressed = true;

    buffer.push(Event(Event::Type::LPress, *this));
    TrimBuffer();
}

void Mouse::OnLeftReleased(const int newX, const int newY) noexcept
{
    isLeftPressed = false;

    buffer.push(Event(Event::Type::LRelease, *this));
    TrimBuffer();
}

void Mouse::OnRightPressed(const int newX, const int newY) noexcept
{
    isRightPressed = true;

    buffer.push(Event(Event::Type::RPress, *this));
    TrimBuffer();
}

void Mouse::OnRightReleased(const int newX, const int newY) noexcept
{
    isRightPressed = false;

    buffer.push(Event(Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(const int newX, const int newY) noexcept
{
    buffer.push(Event(Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(const int newX, const int newY) noexcept
{
    buffer.push(Event(Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
    while (buffer.size() > BUFFER_SIZE)
    {
        buffer.pop();
    }
}

void Mouse::OnWheelDelta(const int newX, const int newY, const int delta) noexcept
{
    wheelDeltaCarry += delta;

    // Events should only occur once every 120 delta
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(newX, newY);
    }
    while (wheelDeltaCarry <= WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(newX, newY);
    }
}
