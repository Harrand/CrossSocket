//
// Created by Harrand on 04/06/2018.
//

#include "address.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

Address::Address(std::string address, std::string port): address(address), port(port){}

Address::Address(std::string address, std::uint16_t port): address(address), port(xsock::address::default_port_string)
{
    std::ostringstream stream;
    stream << port;
    this->port = stream.str();
}

Address::Address(std::string address_with_port)
{
    // e.g "127.0.0.1:25565"
    auto colon_position = address_with_port.find(':');
    this->address = address_with_port.substr(0, colon_position);
    try
    {
        this->port = address_with_port.substr(colon_position + 1);
    }catch(const std::out_of_range& range_exception)
    {
        std::cerr << "IPv4 Address \"" << address_with_port << "\" could not have the port resolved. Resulting to default port.\n";
        this->port = xsock::address::default_port_string;
    }
}

std::string Address::get_full_address() const
{
    return this->address + ":" + this->port;
}

const std::string& Address::get_address() const
{
    return this->address;
}

std::uint16_t Address::get_port() const
{
    std::istringstream iss(this->port);
    std::uint16_t port;
    iss >> port;
    return port;
}

const std::string& Address::get_port_string() const
{
    return this->port;
}

std::ostream& operator<<(std::ostream& str, const Address& address)
{
    str << address.address << ":" << address.port;
    return str;
}
