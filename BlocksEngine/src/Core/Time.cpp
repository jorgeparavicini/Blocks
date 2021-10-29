#include "BlocksEngine/pch.h"
#include "BlocksEngine/Time.h"

BlocksEngine::Time::Time() noexcept(false)
    : deltaTicks_{0},
      totalTicks_{0},
      leftOverTicks_{0},
      frameCount_{0},
      framesPerSecond_{0},
      framesThisSecond_{0},
      qpcSecondCounter_{0},
      isFixedTimeStep_{false},
      targetElapsedTicks_{TicksPerSecond / 60}
{
    if (!QueryPerformanceFrequency(&qpcFrequency_))
    {
        throw std::exception();
    }

    if (!QueryPerformanceCounter(&qpcLastTime_))
    {
        throw std::exception();
    }

    // Initialize max delta to 1/10 of a second.
    qpcMaxDelta_ = static_cast<uint64_t>(qpcFrequency_.QuadPart / 10);
}

uint64_t BlocksEngine::Time::DeltaTicks() const noexcept
{
    return deltaTicks_;
}

double BlocksEngine::Time::DeltaTime() const noexcept
{
    return TicksToSeconds(deltaTicks_);
}

uint64_t BlocksEngine::Time::TotalTicks() const noexcept
{
    return totalTicks_;
}

double BlocksEngine::Time::GetTotalTime() const noexcept
{
    return TicksToSeconds(totalTicks_);
}

uint32_t BlocksEngine::Time::FrameCount() const noexcept
{
    return frameCount_;
}

uint32_t BlocksEngine::Time::FramesPerSecond() const noexcept
{
    return framesPerSecond_;
}

void BlocksEngine::Time::SetFixedTimeStep(const bool isFixedTimestep) noexcept
{
    isFixedTimeStep_ = isFixedTimestep;
}

void BlocksEngine::Time::SetTargetDeltaTicks(const uint64_t targetDelta) noexcept
{
    targetElapsedTicks_ = targetDelta;
}

void BlocksEngine::Time::SetTargetDeltaSeconds(const double targetDelta) noexcept
{
    targetElapsedTicks_ = SecondsToTicks(targetDelta);
}

void BlocksEngine::Time::ResetElapsedTime()
{
    if (!QueryPerformanceCounter(&qpcLastTime_))
    {
        throw std::exception();
    }

    leftOverTicks_ = 0;
    framesPerSecond_ = 0;
    framesThisSecond_ = 0;
    qpcSecondCounter_ = 0;
}
