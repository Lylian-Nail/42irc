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
    if (ac != 3)
        return EXIT_FAILURE;

    Socket socketServer(AddressInfo(av[1], "1080"));
    socketServer.setReusingAddress(true);
    socketServer.bind();
    socketServer.listen();

    std::cout << socketServer << std::endl;
    Socket socketClient = socketServer.accept();
    std::cout << socketClient << std::endl;

    socketClient.close();
    socketServer.close();

    char buffer[32] = "GET /";
    Socket socketTCP(AddressInfo(av[2], "http"));
    socketTCP.connect();
    socketTCP.send(buffer, sizeof("GET /"));

    ssize_t  recvLen = sizeof(buffer) - 1;
    while (recvLen == sizeof(buffer) - 1)
    {
        recvLen = socketTCP.recv(buffer, sizeof(buffer) - 1);
        buffer[recvLen] = '\0';
        std::cout << buffer << std::endl;
    }
}