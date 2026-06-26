//
// Created by Dark on 26/06/2026.
//
#include <iostream>
#include <winsock2.h>
using namespace std;

int main() {
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
}