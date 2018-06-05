#ifndef CROSSSOCKET_ADDRESS_HPP
#define CROSSSOCKET_ADDRESS_HPP
#include <cstdint>
#include <string>

namespace xsock::address
{
    constexpr std::uint16_t default_port = 80;
    constexpr char default_port_string[] = "80";
}

/*
 * A port number is a 16-bit unsigned integer, thus ranging from 0 to 65535.
 * decltype(16-bit unsigned integer) = uint16_t
 */

class Address // IP address (independent of IP version right now, basically a POD)
{
public:
    Address(std::string address, std::uint16_t port);
    Address(std::string address, std::string port);
    Address(std::string address_with_port);
    std::string get_full_address() const;
    const std::string& get_address() const;
    std::uint16_t get_port() const;
    const std::string& get_port_string() const;
    friend std::ostream& operator<<(std::ostream& str, const Address& address);
protected:
    std::string address;
    std::string port;
};


#endif //CROSSSOCKET_ADDRESS_HPP
