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
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], argv[2], &hints, &res);

    if (status != 0) {
        cout << "getaddrinfo failed " << gai_strerror(status) << endl;
        return 2;
    }


}