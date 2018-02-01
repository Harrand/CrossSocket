#include "socket.hpp"
#include <iostream>

namespace sock
{
    bool initialise()
    {
        if constexpr(sock::type == SocketType::WINDOWS)
        {
            // Using Winsock
            std::cout << "Initialising CrossSocket via Windows (Winsock)...";
            WSADATA api_info;
            if(WSAStartup(MAKEWORD(2, 2), &api_info) != 0)
            {
                std::cerr << "Initialisation Failed. Error Code = " << WSAGetLastError();
                return false;
            }
            std::cout << "Initialisation Successful.";
            return true;
        }
        else
        {
            // Using POSIX Socket; anything else will have already failed a static assertation.
            // In addition, POSIX Socket API requires no such initialisation.
            return true;
        }
    }

    void destroy()
    {

    }
}

Socket::Socket(IPVersion ip_version, SocketProtocol protocol): ip_version(ip_version), protocol(protocol)
{
    auto ipv = (this->ip_version == IPVersion::IPV4) ? AF_INET : AF_INET6;
    auto type = (this->protocol == SocketProtocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    if constexpr(sock::type == SocketType::WINDOWS)
    {
        auto proto = (this->protocol == SocketProtocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
        this->socket_handle = socket(ipv, type, proto);
        if(this->socket_handle == INVALID_SOCKET)
            std::cout << "WinSock creation failed. Error Code: " << WSAGetLastError();
    }
    else
    {
        protoent* protocol_entry = getprotobyname(this->protocol == SocketProtocol::TCP ? "tcp" : "udp");
        this->socket_handle = socket(ipv, type, protocol_entry->p_proto);
        if(this->socket_handle < 0)
            std::cout << "POSIX Socket creation failed!";
    }
}