/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AddressInfo.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperson- <lperson-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/29 11:21:25 by lperson-          #+#    #+#             */
/*   Updated: 2022/01/03 14:58:24 by lperson-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDRESS_INFO_HPP

# define ADDRESS_INFO_HPP

# include "IPAddress.hpp"

class AddressInfo
{
public:
    int         m_flags;
    AIPAddress  *m_ipAddress;
    sa_family_t m_ipFamily;
    in_port_t   m_port;
    int         m_sockType;
    int         m_protocol;

    AddressInfo();
    AddressInfo(sa_family_t family, int sockType, int protocol, int flags);
    AddressInfo(
        char const *ipAddress, char const *service,
        AddressInfo const &hint = AddressInfo(0, 0, 0, 0)
    );
    AddressInfo(AddressInfo const &copy);
    ~AddressInfo();

    AddressInfo &operator=(AddressInfo const &rhs);

    class GetAddressInfoException : public std::exception
    {
    public:
        GetAddressInfoException(int errcode);
        const char *what() const throw();

    private:
        int _errcode;
    };

    class NoAddressInfoException : public std::exception
    {
    public:
        const char *what() const throw();
    };

};

#endif
