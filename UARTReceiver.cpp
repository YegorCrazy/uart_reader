#include "UARTReceiver.h"

#include "trackerinterface.h"

std::string UARTReceiver::process()
{
    _lastStateTime = _clock.now();
    char currentChar = 0;
    int bitsReceived = 0;
    ReceiverState currentState = ReceiverState::WaitingForStartBit;

    std::string strCollected = "";

    while (true)
    {
        if (currentState == ReceiverState::WaitingForStartBit)
        {
            if (get_signal_state() == 0)
            {
                currentState = ReceiverState::WaitingForFirstBit;
                _lastStateTime = _clock.now();
            }
        }
        else if (currentState == ReceiverState::WaitingForFirstBit)
        {
            // длина одного бита и еще половина для более 
            // точного пападания в середину следующего бита
            static auto firstBitDuration = _TICK_DURATION * 1.5;
            if (periodPassed(firstBitDuration))
            {
                currentState = ReceiverState::GettingInfoBits;
            }
        }
        else if (currentState == ReceiverState::GettingInfoBits)
        {
            if (periodPassed(_TICK_DURATION))
            {
                currentChar |= (get_signal_state() << (bitsReceived));
                bitsReceived += 1;
                _lastStateTime = _clock.now();
                if (bitsReceived == _BITS_IN_CHAR)
                {
                    strCollected += currentChar;
                    if (*strCollected.rbegin() == 0x0A && *(strCollected.rbegin() + 1) == 0x0D)
                        return strCollected;
                    currentChar = 0;
                    bitsReceived = 0;
                    currentState = ReceiverState::WaitingForStopBit;
                }
            }
        }
        else if (currentState == ReceiverState::WaitingForStopBit)
        {
            if (periodPassed(_TICK_DURATION))
            {
                currentState = ReceiverState::WaitingForStartBit;
            }
        }
    }
}

bool UARTReceiver::periodPassed(std::chrono::duration<double, std::micro> duration)
{
    return _clock.now() - _lastStateTime >= duration;
}