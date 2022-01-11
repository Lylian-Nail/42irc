/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lperson- <lperson-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 15:02:17 by lperson-          #+#    #+#             */
/*   Updated: 2022/01/10 09:15:42 by lperson-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include "AddressInfo.hpp"

#include <arpa/inet.h>

int main(int ac, char const *av[])
{
    if (ac != 2)
        return EXIT_FAILURE;

    AddressInfo infos;
    try
    {
        infos = AddressInfo(av[1], "http");
        std::cout << "IPAddress: " << *infos.m_ipAddress << " port: " <<
        infos.m_port << " protocol: " << infos.m_protocol << " socktype: "
        << infos.m_sockType << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
