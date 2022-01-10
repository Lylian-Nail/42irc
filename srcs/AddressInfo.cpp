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
#include <netdb.h>
#include <cstring>

AddressInfo::AddressInfo():
    m_ipAddress(NULL)
{}

AddressInfo::AddressInfo(
    sa_family_t family, int sockType, int protocol, int flags
):
        m_flags(flags), m_ipAddress(NULL), m_ipFamily(family),
        m_sockType(sockType), m_protocol(protocol)
{}

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
        m_ipFamily = iter->ai_family;
        m_sockType = iter->ai_socktype;
        m_protocol = iter->ai_protocol;
        if (iter->ai_family == AF_INET)
        {
            m_ipAddress = ::getIPAddress(
                reinterpret_cast<struct sockaddr_in *>(iter->ai_addr)
                ->sin_addr
            );
        }
        else if (iter->ai_family == AF_INET6)
        {
            m_ipAddress = ::getIPAddress(
                reinterpret_cast<struct sockaddr_in6 *>(iter->ai_addr)
                ->sin6_addr
            );
        }
        if (m_ipAddress)
            return ;
    }

    throw NoAddressInfoException();
}

AddressInfo::AddressInfo(AddressInfo const &copy):
        m_flags(copy.m_flags), m_ipFamily(copy.m_ipFamily),
        m_sockType(copy.m_sockType), m_protocol(copy.m_protocol)
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
        m_sockType = rhs.m_sockType;
        m_protocol = rhs.m_protocol;
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
