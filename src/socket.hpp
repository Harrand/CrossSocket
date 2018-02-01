#ifndef SOCKET_HPP
#define SOCKET_HPP

#ifdef _WIN32
    #include <winsock2.h>
#elif __unix__ || __APPLE__ || __linux__
    #include <sys/socket.h>
    #include <unistd.h>
#else
    static_assert(false, "Neither POSIX nor WinSock implementations could be identified.");
#endif

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
    ~Socket();
private:
    IPVersion ip_version;
    SocketProtocol protocol;
    sock::Handle socket_handle;
};


#endif //SOCKET_HPP
