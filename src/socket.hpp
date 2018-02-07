#ifndef SOCKET_HPP
#define SOCKET_HPP
#include "address.hpp"
#include <variant>
#include <vector>
#include <cstddef>
#include <string>

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

enum class SocketRole
{
    RECEIVER,
    LISTENER,
    INDETERMINATE
};

namespace xsock
{
    constexpr SocketType type =
    #ifdef _WIN32
        SocketType::WINDOWS;
        using Handle = SOCKET;
    #else
        SocketType::POSIX;
        using Handle = int;
    #endif
    // SOMAXCONN exists in both WinSock and Unix Sockets.
    constexpr int maximum_length_queue = SOMAXCONN;

    bool initialise();
    void destroy();
}

/**
 * Two-way communication link between two programs.
 * API support consists of WinSock and POSIX Sockets.
 * IP support consists of IPv4 and IPv6.
 */
class Socket
{
public:
    Socket(IPVersion ip_version, SocketProtocol protocol, SocketRole role = SocketRole::INDETERMINATE);
    Socket(const Socket& copy);
    Socket(Socket&& move) = default;
    ~Socket();
    bool is_bound() const;
    const SocketRole& get_role() const;
    bool bind_to(const IPv4Address& ipv4_address);
    bool bind_to(const IPv6Address& ipv6_address);
    void listen_requests(int backlog = xsock::maximum_length_queue);
    const std::vector<std::byte>& get_data() const;
    std::string get_data_ascii() const;
    void clear_data();
private:
    bool bound;
    SocketRole role;
    std::vector<std::byte> data_buffer;
    IPVersion ip_version;
    SocketProtocol protocol;
    xsock::Handle socket_handle;
};


#endif //SOCKET_HPP
