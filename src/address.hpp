#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#ifdef _WIN32
    #include <winsock2.h>
    #include <wS2tcpip.h>
    #include <windows.h>
    #define AddressFamily short
    #define Port unsigned short
#elif __unix__ || __APPLE__ || __linux__
    #include <sys/socket.h>
    #include <unistd.h>
    #define AddressFamily sa_family_t
    #define Port in_port_t
#else
    static_assert(false, "Neither POSIX nor WinSock implementations could be identified.");
#endif

#define IP4 in_addr
#define IP6 in6_addr

class Address
{
protected:
    Address(AddressFamily address_family, Port port);
    AddressFamily address_family;
    Port port;
};

class IPv4Address : public Address
{
public:
    IPv4Address(IP4 address, Port port);
    sockaddr_in operator()() const;
private:
    IP4 address;
};

class IPv6Address : public Address
{
public:
    IPv6Address(IP6 address, Port port);
    sockaddr_in6 operator()() const;
private:
    IP6 address;
};


#endif //ADDRESS_HPP
