/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddressInfo.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperson- <lperson-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/29 11:30:45 by lperson-          #+#    #+#             */
/*   Updated: 2022/01/03 14:56:16 by lperson-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AddressInfo.hpp"
#include <sys/types.h>
#include <cstring>

AddressInfo::AddressInfo():
    m_flags(0), m_ipAddress(NULL), m_ipFamily(AF_UNSPEC),
    m_port(0), m_sockType(0), m_protocol(0)
{}

AddressInfo::AddressInfo(
    sa_family_t family, int sockType, int protocol, int flags
):
        m_flags(flags), m_ipAddress(NULL), m_ipFamily(family),
        m_port(0), m_sockType(sockType), m_protocol(protocol)
{}

#include <iostream>

AddressInfo::AddressInfo(
    char const *ipAddress, char const *service,
    AddressInfo const &hint
)
{
    struct addrinfo     cHint;

    std::memset(&cHint, 0, sizeof(cHint));
    cHint.ai_family = hint.m_ipFamily;
    cHint.ai_socktype = hint.m_sockType;
    cHint.ai_protocol = hint.m_protocol;
    cHint.ai_flags = hint.m_flags;

    struct addrinfo     *results;
    int errcode = getaddrinfo(
        ipAddress, service, &cHint, &results
    );
    if (errcode != 0)
        throw GetAddressInfoException(errcode);

    m_ipAddress = NULL;
    for (
        struct addrinfo *iter = results;
        iter != NULL;
        iter = iter->ai_next
    )
    {
        m_cStyle = *iter;
        m_cStyle.ai_next = NULL;
        m_ipFamily = iter->ai_family;
        m_sockType = iter->ai_socktype;
        m_protocol = iter->ai_protocol;
        if (iter->ai_family == AF_INET)
        {
            m_ipAddress = ::getIPAddress(
                reinterpret_cast<struct sockaddr_in *>(iter->ai_addr)
                ->sin_addr
            );
            m_port = reinterpret_cast<struct sockaddr_in *>(iter->ai_addr)
                    ->sin_port;
        }
        else if (iter->ai_family == AF_INET6)
        {
            m_ipAddress = ::getIPAddress(
                reinterpret_cast<struct sockaddr_in6 *>(iter->ai_addr)
                ->sin6_addr
            );
            m_port = reinterpret_cast<struct sockaddr_in6 *>(iter->ai_addr)
                    ->sin6_port;
        }
        // Return when finding 1st result
        if (m_ipAddress)
        {
            freeaddrinfo(results);
            return ;
        }
    }

    freeaddrinfo(results);
    throw NoAddressInfoException()
}

AddressInfo::AddressInfo(AddressInfo const &copy):
        m_flags(copy.m_flags), m_ipFamily(copy.m_ipFamily), m_port(copy.m_port),
        m_sockType(copy.m_sockType), m_protocol(copy.m_protocol),
        m_cStyle((copy.m_cStyle))
{
    if (copy.m_ipAddress != NULL)
        m_ipAddress = copy.m_ipAddress->clone();
    else
        m_ipAddress = NULL;
}

AddressInfo::~AddressInfo()
{
    delete m_ipAddress;
}

AddressInfo &AddressInfo::operator=(AddressInfo const &rhs)
{
    if (this != &rhs)
    {
        m_flags = rhs.m_flags;
        delete m_ipAddress;
        if (rhs.m_ipAddress != NULL)
            m_ipAddress = rhs.m_ipAddress->clone();
        else
            m_ipAddress = NULL;
        m_ipFamily = rhs.m_ipFamily;
        m_port = rhs.m_port;
        m_sockType = rhs.m_sockType;
        m_protocol = rhs.m_protocol;
        m_cStyle = rhs.m_cStyle;
    }
    return *this;
}

AddressInfo::GetAddressInfoException::GetAddressInfoException(int errcode):
        _errcode(errcode)
{}

const char *AddressInfo::GetAddressInfoException::what() const throw()
{
    return gai_strerror(_errcode);
}

const char *AddressInfo::NoAddressInfoException::what() const throw()
{
    return "no address info founds";
}

std::ostream &operator<<(std::ostream &os, AddressInfo const &infos)
{
    std::string outputIP = infos.m_ipAddress ?
            infos.m_ipAddress->getRepresentation() : "no ip address";

    os << "IP Address: " << outputIP << ", family: ";
    switch (infos.m_ipFamily) {
        case AF_INET:
            os << "IPv4";
            break;
        case AF_INET6:
            os << "IPv6";
            break;
        default:
            os << infos.m_ipFamily;
    }
    os << ", port: " << ntohs(infos.m_port) << ", sock type: ";
    switch (infos.m_sockType) {
        case SOCK_DGRAM:
            os << "Datagram";
            break;
        case SOCK_STREAM:
            os << "Stream";
            break;
        default:
            os << infos.m_sockType;
    }
    os << ", protocol: ";
    switch (infos.m_protocol) {
        case IPPROTO_TCP:
            os << "TCP";
            break;
        case IPPROTO_UDP:
            os << "UDP";
            break;
        default:
            os << infos.m_protocol;
    }

    return os;
}