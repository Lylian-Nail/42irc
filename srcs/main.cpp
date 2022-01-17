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
    if (ac != 2)
        return EXIT_FAILURE;

    Socket socket1(AddressInfo(av[1], "http"));

    std::string truc("GET /");
    ssize_t len = socket1.send(truc.c_str(), truc.length());
    std::cout << len << std::endl;

    char buffer[1000];
    len = socket1.recv(buffer, sizeof(buffer) - 1);
    if (len >= 0)
    {
        buffer[len] = '\0';
        std::cout << buffer << std::endl;
    }

    socket1.close();
}