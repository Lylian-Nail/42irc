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
#include "Socket.hpp"

int main(int ac, char const *av[])
{
    Socket socket1;

    if (ac != 2)
        return EXIT_FAILURE;

    try
    {
        socket1 = Socket(AddressInfo(av[1], "http"));
        std::cout << socket1 << std::endl;
        socket1.setBlocking(false);
        socket1.setReusingAddress(true);
        std::cout << socket1 << std::endl;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}