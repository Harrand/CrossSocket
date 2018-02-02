#include "socket.hpp"
#include <iostream>

namespace xsock
{
    bool initialise()
    {
        if constexpr(xsock::type == SocketType::WINDOWS)
        {
            // Using WinSock
            std::cout << "Initialising CrossSocket via Windows (WinSock)...";
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
        if constexpr(xsock::type == SocketType::WINDOWS)
        {
            WSACleanup();
            std::cout << "WinSock cleanup successful.";
        }
        else
            std::cout << "POSIX cleanup successful.";
    }
}

Socket::Socket(IPVersion ip_version, SocketProtocol protocol): bound(false), data_buffer({}), ip_version(ip_version), protocol(protocol)
{
    auto ipv = (this->ip_version == IPVersion::IPV4) ? AF_INET : AF_INET6;
    auto type = (this->protocol == SocketProtocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    if constexpr(xsock::type == SocketType::WINDOWS)
    {
        auto proto = (this->protocol == SocketProtocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP;
        this->socket_handle = socket(ipv, type, proto);
        if(this->socket_handle == INVALID_SOCKET)
            std::cout << "WinSock Socket creation failed. Error Code: " << WSAGetLastError();
    }
    else
    {
        protoent* protocol_entry = getprotobyname(this->protocol == SocketProtocol::TCP ? "tcp" : "udp");
        this->socket_handle = socket(ipv, type, protocol_entry->p_proto);
        if(this->socket_handle < 0)
            std::cout << "POSIX Socket creation failed. No error-code is available.";
    }
}

Socket::Socket(const Socket& copy): Socket(copy.ip_version, copy.protocol) {}

Socket::~Socket()
{
    #ifdef _WIN32
        closesocket(this->socket_handle);
    #else
        close(this->socket_handle);
    #endif
}

bool Socket::is_bound() const
{
    return this->bound;
}

bool Socket::bind_to(const IPv4Address& ip_address)
{
    sockaddr_in api_address = ip_address();
    if(bind(this->socket_handle, reinterpret_cast<const sockaddr*>(&api_address), sizeof(api_address)) == 0)
    {
        this->bound = true;
        return true;
    }
    std::cerr << "WinSock Socket IPv4 bind failed. Error-code: " << WSAGetLastError();
    return false;
}

bool Socket::bind_to(const IPv6Address& ip_address)
{
    sockaddr_in6 api_address = ip_address();
    if(bind(this->socket_handle, reinterpret_cast<const struct sockaddr*>(&api_address), sizeof(api_address)) == 0)
    {
        this->bound = true;
        return true;
    }
    std::cerr << "WinSock Socket IPv6 bind failed. Error-code: " << WSAGetLastError();
    return false;
}

const std::vector<std::byte>& Socket::get_data() const
{
    return this->data_buffer;
}

std::string Socket::get_data_ascii() const
{
    std::string data = "";
    data.reserve(this->get_data().size());
    for(const std::byte& byte : this->get_data())
        data += static_cast<char>(byte);
    return data;
}

void Socket::clear_data()
{
    this->data_buffer.clear();
}