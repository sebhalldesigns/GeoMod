#ifndef GKERNELSERVER_H
#define GKERNELSERVER_H

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <cstdio>
    #include <cstdlib>
#endif // WIN32

#include "../Debug/gLog.h"

#include <string>
#include <optional>

const int BUFFER_SIZE = 1024;
const char* LISTEN_PORT_NO = "2023";


class gKernelServer {

    static SOCKET sock;
    static struct sockaddr_in addr;

public:

    gKernelServer() {
        sock = INVALID_SOCKET;
    }

    bool start(const std::string& ip, int port) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (result != 0) {
            gLog::Error("WSAStartup failed: %d", result);
            return false;
        }

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            gLog::Error("Error at socket(): %d", WSAGetLastError());
            WSACleanup();
            return false;
        }

        u_long mode = 1;  // Enable non-blocking mode
        result = ioctlsocket(sock, FIONBIO, &mode);
        if (result == SOCKET_ERROR) {
            gLog::Error("ioctlsocket failed with error: %d", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return false;
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        addr.sin_port = htons(static_cast<u_short>(port));

        result = bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
        if (result == SOCKET_ERROR) {
            gLog::Error("bind failed with error: %d", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return false;
        }

        result = listen(sock, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            gLog::Error("listen failed with error: %d", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return false;
        }

        return true;
    }

    void stop() {
        if (sock != INVALID_SOCKET) {
            closesocket(sock);
            sock = INVALID_SOCKET;
        }
        WSACleanup();
    }

    std::optional<std::string> poll() {
        SOCKET clientSock = accept(sock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) {
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                gLog::Error("accept failed with error: %d", WSAGetLastError());
            }
            return std::nullopt;
        }

        char buffer[512];
        int bytesReceived = recv(clientSock, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                gLog::Error("recv failed with error: %d", WSAGetLastError());
            }
            closesocket(clientSock);
            return std::nullopt;
        }

        std::string message(buffer, bytesReceived);
        closesocket(clientSock);
        return message;
    }
};


#endif // GKERNELSERVER_H