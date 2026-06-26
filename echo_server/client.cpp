//
// Created by Dark on 26/06/2026.
//
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
using namespace std;

int main(int argc, char* argv[]) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "WSAStartup failed" << endl;
        return 1;
    }
    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2 ) {
        cout << "LOBYTE failed" << endl;
        return 2;
        }

    if (argc != 3) {
        cout << "Invalid argument provided";
        WSACleanup();
        return 1;
    }

    int status;
    struct addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], argv[2], &hints, &res);

    if (status != 0) {
        cerr << "getaddrinfo failed " << gai_strerror(status) << endl;
        WSACleanup();
        return 2;
    }

    //creating a socket

    int socketfd;
    for (p = res; p != nullptr; p = p->ai_next) {
        socketfd = socket (p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketfd == INVALID_SOCKET) {
            cerr << "Error creating socket" << endl;
            continue;
        }
        if (connect (socketfd,p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {       //connect to server
            cerr << "Error connecting to socket" << endl;
            closesocket(socketfd);
            continue;
        }
        break;
    }

    if (p == nullptr) {
        cout << "Connection failed" << endl;
        WSACleanup();
        freeaddrinfo(res);
        return 2;
    }

    const char* msg = "Hello from client";
    int len, bytes_sent;
    len = strlen(msg);
    if ((bytes_sent = send (socketfd, msg, len, 0)) == SOCKET_ERROR) {
        cerr << "Error writing to socket" << endl;
        WSACleanup();
        freeaddrinfo(res);
        return 2;
    };

    char buff[256];
    int rec;
    if ((rec = recv(socketfd, buff, sizeof(buff) -1, 0)) == SOCKET_ERROR) {
        cerr << "Error reading from socket" << endl;
        WSACleanup();
        freeaddrinfo(res);
        return 2;
    }
    if (rec == ( 0 || SOCKET_ERROR)) {
        cerr << "Error reading from socket" << endl;
        WSACleanup();
        freeaddrinfo(res);
        return 1;
    }

    buff[rec] = '\0';
    closesocket(socketfd);

    freeaddrinfo(res);
    WSACleanup();
    return 0;
}