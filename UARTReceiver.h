#pragma once

#include <chrono>
#include <string>

class UARTReceiver
{
public:
    std::string process();

private:
    enum class ReceiverState
    {
        WaitingForStartBit = 0,
        WaitingForFirstBit = 1,
        GettingInfoBits = 2,
        WaitingForStopBit = 3,
    };

    bool periodPassed(std::chrono::duration<double, std::micro> duration);

    static constexpr int _BITS_PER_SECOND = 1200;
    static constexpr int _MICROSECOND_PER_SECOND = 1'000'000;
    static constexpr int _BITS_IN_CHAR = 8;
    static constexpr std::chrono::duration<double, std::micro> _TICK_DURATION = 
        std::chrono::duration<double, std::micro>(_MICROSECOND_PER_SECOND / _BITS_PER_SECOND);

    std::chrono::steady_clock _clock;
    std::chrono::time_point<std::chrono::steady_clock> _lastStateTime;
};