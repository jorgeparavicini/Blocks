// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Time.h

// Based on StepTimer.h from DirectXTK

#pragma once

#include <cmath>
#include <cstdint>
#include <exception>
#include <winnt.h>

namespace BlocksEngine
{
    class Time;
}

class BlocksEngine::Time
{
public:
    Time() noexcept(false);

    /**
     * \brief Get the number of elapsed Ticks since the previous Update call.
     * \return The number of elapsed Ticks.
     */
    [[nodiscard]] uint64_t DeltaTicks() const noexcept;

    /**
     * \brief Get the fraction of elapsed time in seconds since the previous Update call.
     * \return The number of elapsed seconds.
     */
    [[nodiscard]] double DeltaTime() const noexcept;

    /**
     * \brief Get the total number of ticks since the beginning of the game.
     * \return The total number of ticks.
     */
    [[nodiscard]] uint64_t TotalTicks() const noexcept;

    /**
     * \brief Get the total amount of time in seconds since the beginning of the game.
     * \return The total amount of time in seconds.
     */
    [[nodiscard]] double GetTotalTime() const noexcept;


    /**
     * \brief Get total number of updates since the start of the game.
     * \return The total number of updates.
     */
    [[nodiscard]] uint32_t FrameCount() const noexcept;

    /**
     * \brief Get the current framerate.
     * \return The current framerate measured per second.
     */
    [[nodiscard]] uint32_t FramesPerSecond() const noexcept;

    /**
     * \brief Set whether to use fixed or variable timestep mode.
     * \param isFixedTimestep If set to true the game will use a the fixed timestep mode. Otherwise the timestep will be variable.
     */
    void SetFixedTimeStep(bool isFixedTimestep) noexcept;

    /**
     * \brief Set how often to call Update. Only works if in fixed timestep mode.
     * \param targetDelta How many times Update should be called for one tick operation.
     */
    void SetTargetDeltaTicks(uint64_t targetDelta) noexcept;

    /**
     * \brief Set how often to call Update. Only works if in fixed timestep mode.
     * \param targetDelta How many times Update should be called so every tick gets called after targetDelta seconds.
     */
    void SetTargetDeltaSeconds(double targetDelta) noexcept;


    /**
     * \brief Integer format represents time using 10,000,000 ticks per second.
     */
    static constexpr uint64_t TicksPerSecond = 10000000;

    /**
     * \brief Converts a given number of ticks to the corresponding number of seconds.
     * \param ticks The number of ticks to be converted.
     * \return The corresponding number of seconds.
     */
    static constexpr double TicksToSeconds(const uint64_t ticks) noexcept
    {
        return static_cast<double>(ticks) / TicksPerSecond;
    }

    /**
     * \brief Converts a given number of seconds to the corresponding number of ticks.
     * \param seconds The number of seconds to be converted.
     * \return The corresponding number of ticks.
     */
    static constexpr uint64_t SecondsToTicks(const double seconds) noexcept
    {
        return static_cast<uint64_t>(seconds * TicksPerSecond);
    }

    /**
     * \brief After an intentional timing discontinuity (for instance a blocking IO operation)
     * call this to avoid having the fixed timestep logic attempt a set of catch-up Update calls.
     */
    void ResetElapsedTime();


    /**
     * \brief Update timer state, calling the specified Update function the appropriate number of times.
     * \tparam TUpdate The type of the update function to be called.
     * \param update The update function of type TUpdate to be called.
     */
    template <typename TUpdate>
    void Tick(const TUpdate& update)
    {
        // Query the current time.
        LARGE_INTEGER currentTime;

        if (!QueryPerformanceCounter(&currentTime))
        {
            throw std::exception();
        }

        uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - qpcLastTime_.QuadPart);

        qpcLastTime_ = currentTime;
        qpcSecondCounter_ += timeDelta;

        // Clamp excessively large time deltas (e.g. after paused in the debugger).
        if (timeDelta > qpcMaxDelta_)
        {
            timeDelta = qpcMaxDelta_;
        }

        // Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
        timeDelta *= TicksPerSecond;
        timeDelta /= static_cast<uint64_t>(qpcFrequency_.QuadPart);

        const uint32_t lastFrameCount = frameCount_;

        if (isFixedTimeStep_)
        {
            // Fixed timestep update logic

            // If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
            // the clock to exactly match the target value. This prevents tiny and irrelevant errors
            // from accumulating over time. Without this clamping, a game that requested a 60 fps
            // fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
            // accumulate enough tiny errors that it would drop a frame. It is better to just round
            // small deviations down to zero to leave things running smoothly.

            if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - targetElapsedTicks_))) < TicksPerSecond
                / 4000)
            {
                timeDelta = targetElapsedTicks_;
            }

            leftOverTicks_ += timeDelta;

            while (leftOverTicks_ >= targetElapsedTicks_)
            {
                deltaTicks_ = targetElapsedTicks_;
                totalTicks_ += targetElapsedTicks_;
                leftOverTicks_ -= targetElapsedTicks_;
                frameCount_++;

                update();
            }
        }
        else
        {
            // Variable timestep update logic.
            deltaTicks_ = timeDelta;
            totalTicks_ += timeDelta;
            leftOverTicks_ = 0;
            frameCount_++;

            update();
        }

        // Track the current framerate.
        if (frameCount_ != lastFrameCount)
        {
            framesThisSecond_++;
        }

        if (qpcSecondCounter_ >= static_cast<uint64_t>(qpcFrequency_.QuadPart))
        {
            framesPerSecond_ = framesThisSecond_;
            framesThisSecond_ = 0;
            qpcSecondCounter_ %= static_cast<uint64_t>(qpcFrequency_.QuadPart);
        }
    }

private:
    // Source timing data uses QPC units.
    LARGE_INTEGER qpcFrequency_{};
    LARGE_INTEGER qpcLastTime_{};
    uint64_t qpcMaxDelta_;

    // Derived timing data uses a canonical tick format.
    uint64_t deltaTicks_;
    uint64_t totalTicks_;
    uint64_t leftOverTicks_;

    // Members for tracking the framerate.
    uint32_t frameCount_;
    uint32_t framesPerSecond_;
    uint32_t framesThisSecond_;
    uint64_t qpcSecondCounter_;

    // Members for configuring fixed timestep mode.
    bool isFixedTimeStep_;
    uint64_t targetElapsedTicks_;
};
