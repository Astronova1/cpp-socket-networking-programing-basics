//
// Created by Dark on 26/06/2026.
//
#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <ws2tcpip.h>
#include <cstdlib>

# define BACKLOG 5

int main (int argc, char* argv[]) {

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        std::cerr << "WSAStartup() failed." << std::endl;
        WSAGetLastError();
        WSACleanup();
        return 2;
    }
    if (LOBYTE (wsaData.wVersion) != 2 ||
        HIBYTE (wsaData.wVersion) != 2 ) {
        std::cerr << "WSAStartup() failed";
        WSAGetLastError();
        WSACleanup();
        return 2;
    }

    int status;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        WSAGetLastError();
        WSACleanup();
    }

    status = getaddrinfo(nullptr, argv[1], &hints, &res );

    if (status != 0) {
        std::cerr << "getaddrinfo() failed." << std::endl;
        WSAGetLastError();
        WSACleanup();
        return 1;
    }
    int socketfd, new_fd, binded, yes=1;
    for (p = res; p != nullptr; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == INVALID_SOCKET) {
            std::cerr << "socket() failed." << std::endl;
            WSAGetLastError();
            closesocket(socketfd);
            continue;
        }
        // If the port binding is already in use , allows reuse of port
        setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int));
                                                        //binding the server to port for server
        if (bind(socketfd,p->ai_addr,p->ai_addrlen) == SOCKET_ERROR) {
            std::cerr << "bind() failed." << std::endl << WSAGetLastError();
            closesocket(socketfd);
            continue;
        }
        break;
    }
    if (p == nullptr) {
        std::cerr << "connection Failed!\n" << WSAGetLastError();
        freeaddrinfo(res);
        closesocket(socketfd);
        return 2;
    }

    if (listen(socketfd,BACKLOG) != 0 ) {
        std::cerr << "listen() failed." << std::endl;
        WSAGetLastError();
        freeaddrinfo(res);
        return 2;
    }

    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    //accepting an incoming traffic
    addr_size = sizeof client_addr;
    new_fd = accept(socketfd, (struct sockaddr*)&client_addr, &addr_size);

    int rec_client;
    char buff[256];

    if ((rec_client = recv(socketfd,buff , sizeof(buff) -1, 0)) != 0) {
        std::cerr << "accept() failed." << std::endl;
        WSAGetLastError();
        closesocket(new_fd);
        freeaddrinfo(res);
        return 2;
    }
    buff[rec_client] = '\0';
    std::cout << buff << std::endl;

    send(new_fd, buff, rec_client, 0);

    closesocket(new_fd);
    WSACleanup();
    freeaddrinfo(res);
    return 0;
}