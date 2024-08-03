#include <iostream>
#include <chrono>
#include <ratio>

#include "trackerinterface.h"

enum class ReceiverState
{
    WaitingForStartBit = 0,
    WaitingForFirstBit = 1,
    GettingInfoBits = 2,
    WaitingForStopBit = 3,
};

const int BITS_PER_SECOND = 1200;
const int MICROSECOND_PER_SECOND = 1'000'000;
const int BITS_IN_CHAR = 8;

int main()
{
    std::chrono::duration<double, std::micro> tickDuration(
        MICROSECOND_PER_SECOND / BITS_PER_SECOND
    );
    auto clock = std::chrono::steady_clock();
    auto lastStateTime = clock.now();
    char currentChar = 0;
    int bitsReceived = 0;
    ReceiverState currentState = ReceiverState::WaitingForStartBit;

    while (true)
    {
        if (currentState == ReceiverState::WaitingForStartBit)
        {
            if (get_signal_state() == 0)
            {
                currentState = ReceiverState::WaitingForFirstBit;
                lastStateTime = clock.now();
            }
        }
        else if (currentState == ReceiverState::WaitingForFirstBit)
        {
            // длина одного бита и еще половина для более 
            // точного пападания в середину следующего бита
            static auto firstBitDuration = tickDuration * 1.5;
            if (clock.now() - lastStateTime >= firstBitDuration)
            {
                currentState = ReceiverState::GettingInfoBits;
            }
        }
        else if (currentState == ReceiverState::GettingInfoBits)
        {
            if (clock.now() - lastStateTime >= tickDuration)
            {
                currentChar |= (get_signal_state() << (bitsReceived));
                bitsReceived += 1;
                lastStateTime = clock.now();
                if (bitsReceived == BITS_IN_CHAR)
                {
                    std::cout << currentChar;
                    currentChar = 0;
                    bitsReceived = 0;
                    currentState = ReceiverState::WaitingForStopBit;
                }
            }
        }
        else if (currentState == ReceiverState::WaitingForStopBit)
        {
            if (clock.now() - lastStateTime >= tickDuration)
            {
                currentState = ReceiverState::WaitingForStartBit;
            }
        }
    }
    
    return 0;
}

