/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPAddress.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperson- <lperson-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 15:19:30 by lperson-          #+#    #+#             */
/*   Updated: 2021/12/23 13:00:42 by lperson-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IPAddress.hpp"
#include <arpa/inet.h>

/*
 * Abstract ip address implementation
*/

AIPAddress::AIPAddress():
        _representation(""),
        _family(AF_UNSPEC),
        _cStyle(NULL)
{}

AIPAddress::AIPAddress(std::string const &ipAddress, sa_family_t family):
        _representation(ipAddress),
        _family(family),
        _cStyle(NULL)
{}

AIPAddress::AIPAddress(AIPAddress const &copy):
        _representation(copy.getRepresentation()),
        _family(copy.getFamily()),
        _cStyle(copy.getCStyle())
{}

// Since we're using polymorphism not needed copy
AIPAddress &AIPAddress::operator=(AIPAddress const &)
{
    return *this;
}

std::string AIPAddress::getRepresentation() const
{
    return _representation;
}

sa_family_t AIPAddress::getFamily() const
{
    return _family;
}

void const *AIPAddress::getCStyle() const
{
    return _cStyle;
}

AIPAddress::~AIPAddress()
{}

/*
 * IPv4 address implementation
*/

IPv4Address::IPv4Address()
{}

IPv4Address::IPv4Address(std::string const &ipAddress):
        AIPAddress(ipAddress, AF_INET)
{
    int ret = inet_pton(
        getFamily(),
        getRepresentation().c_str(),
        reinterpret_cast<void *>(&_addr)
    );
    if (ret != 1)
        throw AddressValueException();

    _cStyle = reinterpret_cast<void const *>(&_addr);
}

IPv4Address::IPv4Address(IPv4Address const &copy):
        AIPAddress(copy)
{}

IPv4Address &IPv4Address::operator=(IPv4Address const &)
{
    return *this;
}

AIPAddress *IPv4Address::clone() const
{
    return new IPv4Address(*this);
}

IPv4Address::~IPv4Address()
{}

/*
 * IPv4 AddressValueException implementation
*/

const char *IPv4Address::AddressValueException::what() const throw()
{
    return "invalid ipv4 address value";
}

/*
 * IPv6 address implementation
*/

IPv6Address::IPv6Address()
{}

IPv6Address::IPv6Address(std::string const &string):
        AIPAddress(string, AF_INET6)
{
    int ret = inet_pton(
        getFamily(),
        getRepresentation().c_str(),
        reinterpret_cast<void *>(&_addr)
    );
    if (ret != 1)
        throw AddressValueException();

    _cStyle = reinterpret_cast<void const *>(&_addr);
}

IPv6Address::IPv6Address(IPv6Address const &copy): AIPAddress(copy)
{}

IPv6Address &IPv6Address::operator=(IPv6Address const &)
{
    return *this;
}

AIPAddress *IPv6Address::clone() const
{
    return new IPv6Address(*this);
}

IPv6Address::~IPv6Address()
{}

/*
 * IPv6 AddressValueException implementation
*/

const char *IPv6Address::AddressValueException::what() const throw()
{
    return "invalid ipv6 address value";
}

/*
 * Producer of ip address implementation
*/

AIPAddress *getIPAddress(std::string const &ipAddress)
{
    try
    {
        return new IPv4Address(ipAddress);
    }
    catch (std::exception const &)
    {
        ; // Simply ignore and then try ipv6
    }

    try
    {
        return new IPv6Address(ipAddress);
    }
    catch (std::exception const &)
    {
        return NULL;
    }
}
