//
// Created by Harrand on 04/06/2018.
//

#include <iostream>
#include "socket.hpp"
Socket::Socket(Address address): address(address){}

#ifdef _WIN32
WindowsSocket::WindowsSocket(Address address, xsock::Protocol protocol): Socket(address), socket_info(), hints(), result(nullptr), protocol(protocol), other(nullptr)
{
    using namespace xsock;
    // haven't yet defined whether IPv4 or IPv6
    socket_info.ai_family = AF_UNSPEC;
    switch(this->protocol)
    {
        case Protocol::TCP:
            socket_info.ai_socktype = SOCK_STREAM;
            socket_info.ai_protocol = IPPROTO_TCP;
            break;
        case Protocol::UDP:
            socket_info.ai_socktype = SOCK_DGRAM;
            socket_info.ai_protocol = IPPROTO_UDP;
            break;
    }
    // TODO: Error Handling
    getaddrinfo(this->address.get_address().c_str(), this->address.get_port_string().c_str(), &this->hints, &this->result);
}

WindowsSocket::~WindowsSocket()
{
    closesocket(this->socket_handle);
}

bool WindowsSocket::valid() const
{
    return this->socket_handle != INVALID_SOCKET;
}

bool WindowsSocket::connect_socket(WindowsSocket& other)
{
    this->other = &other;
    for(addrinfo* ptr = this->result; ptr != NULL; ptr = ptr->ai_next)
    {
        std::cout << "TEST\n";
        this->socket_handle = socket(this->socket_info.ai_family, this->socket_info.ai_socktype, this->socket_info.ai_protocol);
        if (!this->valid())
        {
            std::cerr << "Error: Windows Socket bind did not result in a valid socket:\n";
            std::cerr << WSAGetLastError() << "\n";
            return false;
        }
        int result = connect(this->socket_handle, other.socket_info.ai_addr, static_cast<int>(other.socket_info.ai_addrlen));
        if(result == SOCKET_ERROR)
        {
            closesocket(this->socket_handle);
            this->socket_handle = INVALID_SOCKET;
            continue;
        }
        return true;
    }
    std::cerr << "Error: None of the addresses could be connected to.\n";
    return false;
}

bool WindowsSocket::send_bytes(const char* bytes) const
{
    return send(this->socket_handle, bytes, static_cast<int>(strlen(bytes)), 0) != SOCKET_ERROR;
}

std::optional<std::vector<char>> WindowsSocket::receive_bytes(int number_of_bytes)
{
    std::vector<char> receive_buffer;
    receive_buffer.resize(static_cast<std::size_t>(number_of_bytes));
    int result = recv(this->socket_handle, receive_buffer.data(), number_of_bytes, 0);
    if(result >= 0)
        return receive_buffer;
    else if(result == SOCKET_ERROR)
    {
        std::cerr << "Error: Windows Socket receive method received an error:\n";
        std::cerr << WSAGetLastError() << "\n";
        return std::nullopt;
    } else{std::cerr << "uh oh.\n";}
}

WindowsIPv4Socket::WindowsIPv4Socket(Address address, xsock::Protocol protocol): WindowsSocket(address, protocol)
{
    this->socket_info.ai_family = AF_INET;
}

WindowsIPv6Socket::WindowsIPv6Socket(Address address, xsock::Protocol protocol): WindowsSocket(address, protocol)
{
    this->socket_info.ai_family = AF_INET6;
}

#endif