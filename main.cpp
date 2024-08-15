#include <iostream>

#include <netinet/in.h>
#include <sys/socket.h>

#include "UARTReceiver.h"

const unsigned short UDP_PORT = 5678;
const unsigned UDP_IP = 0x7F000001; // 127.0.0.1

int main()
{
    auto socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFd == -1)
    {
        std::cout << "Bad socket file descriptor!" << std::endl;
        std::cout << "errno is " << errno << std::endl;
        return 1;
    }
    auto socketIPAddressDst = sockaddr_in{AF_INET, htons(UDP_PORT), htonl(UDP_IP)};

    auto uartReceiver = UARTReceiver();
    while (true)
    {
        auto strReceived = uartReceiver.process();
        std::cout << strReceived;
        if (sendto(socketFd, strReceived.c_str(), strReceived.size(), 0,
            (sockaddr*)(&socketIPAddressDst), sizeof(socketIPAddressDst)) != strReceived.size())
        {
            std::cout << "Bad socket send!" << std::endl;
            std::cout << "errno is " << errno << std::endl;
            return 4;
        }

    }
    return 0;
}

