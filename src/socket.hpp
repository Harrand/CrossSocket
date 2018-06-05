#ifndef CROSSSOCKET_SOCKET_HPP
#define CROSSSOCKET_SOCKET_HPP
#include "address.hpp"

#ifdef _WIN32
#include <ws2tcpip.h>
#include <windows.h>
#include <cstddef>
#include <vector>
#include <optional>

#elif __unix__ || __APPLE__ || __linux__
    #include <sys/socket.h>
    #include <unistd.h>
    #define AddressFamily sa_family_t
    #define Port in_port_t
#else
    static_assert(false, "Neither POSIX nor WinSock implementations could be identified.");
#endif

namespace xsock
{
    enum class Protocol{TCP, UDP};
}

class Socket
{
public:
    Socket(Address address);
    virtual bool valid() const = 0;
protected:
    Address address;
};

#ifdef _WIN32
class WindowsSocket : public Socket
{
public:
    WindowsSocket(Address address, xsock::Protocol protocol);
    virtual ~WindowsSocket();
    virtual bool valid() const override;
    virtual bool connect_socket(WindowsSocket& other);
    bool send_bytes(const char* bytes) const;
    std::optional<std::vector<char>> receive_bytes(int number_of_bytes);
protected:
    addrinfo socket_info, hints;
    addrinfo* result;
    xsock::Protocol protocol;
    SOCKET socket_handle;
    WindowsSocket* other;
};

class WindowsIPv4Socket : public WindowsSocket
{
public:
    WindowsIPv4Socket(Address address, xsock::Protocol protocol);
};

class WindowsIPv6Socket : public WindowsSocket
{
public:
    WindowsIPv6Socket(Address address, xsock::Protocol protocol);
};
#endif

#endif //CROSSSOCKET_SOCKET_HPP
