#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "address.hpp"
#include <variant>

enum class SocketType : unsigned int
{
    WINDOWS,
    POSIX
};

enum class IPVersion : unsigned int
{
    IPV4,
    IPV6
};

enum class SocketProtocol : unsigned int
{
    TCP,
    UDP
};

namespace sock
{
    constexpr SocketType type =
    #ifdef _WIN32
        SocketType::WINDOWS;
        using Handle = SOCKET;
    #else
        SocketType::POSIX;
        using Handle = int;
    #endif

    bool initialise();
    void destroy();
}

class Socket
{
public:
    Socket(IPVersion ip_version, SocketProtocol protocol);
    Socket(const Socket& copy);
    Socket(Socket&& move) = default;
    ~Socket();
    bool is_bound() const;
    bool bind_to(const IPv4Address& ipv4_address);
    bool bind_to(const IPv6Address& ipv6_address);
private:
    bool bound;
    IPVersion ip_version;
    SocketProtocol protocol;
    sock::Handle socket_handle;
};


#endif //SOCKET_HPP
