//
// Created by Dark on 26/06/2026.
//
#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <ws2tcpip.h>
#include <cstdlib>


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

    status = getaddrinfo(nullptr, argv[1], &hints, &res );
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        WSAGetLastError();
        WSACleanup();
    }
    if (status != 0) {
        std::cerr << "getaddrinfo() failed." << std::endl;
        WSAGetLastError();
        WSACleanup();
        return 1;
    }
    int socketfd, binded;
    for (p = res; p != nullptr; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == INVALID_SOCKET) {
            std::cerr << "socket() failed." << std::endl;
            WSAGetLastError();
            closesocket(socketfd);
            continue;
        }                                       //binding the server to port for server
        if (bind(socketfd,p->ai_addr,p->ai_addrlen) == SOCKET_ERROR) {
            std::cerr << "bind() failed." << std::endl << WSAGetLastError();
            closesocket(socketfd);
            continue;
        }
        break;
    }

    WSACleanup();
    freeaddrinfo(res);
}