#include "address.hpp"

Address::Address(AddressFamily address_family, Port port): address_family(address_family), port(port){}

IPv4Address::IPv4Address(IP4 address, Port port): Address(AF_INET, port) {}

sockaddr_in IPv4Address::operator()() const
{
    sockaddr_in api_address;
    api_address.sin_family = this->address_family;
    api_address.sin_port = htons(this->port);
    api_address.sin_addr = this->address;
    return api_address;
}

IPv6Address::IPv6Address(IP6 address, Port port): Address(AF_INET6, port) {}

sockaddr_in6 IPv6Address::operator()() const
{
    sockaddr_in6 api_address;
    api_address.sin6_family = this->address_family;
    api_address.sin6_port = htons(this->port);
    api_address.sin6_addr = this->address;
    return api_address;
}