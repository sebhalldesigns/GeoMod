#ifndef GKERNELSERVER_H
#define GKERNELSERVER_H

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <winsock2.h>
    #include <afunix.h>
    #include <ws2tcpip.h>
    #include <cstdio>
    #include <cstdlib>
    #include <filesystem>
#endif // WIN32

#include "../Debug/gLog.h"

#include <string>
#include <optional>

const int BUFFER_SIZE = 1024;
const char* LISTEN_PORT_NO = "2023";


class gKernelServer {

    static SOCKET serverSocket;
    static struct sockaddr_un serverAddress;
    static std::string path;

public:

    gKernelServer() {
        serverSocket = INVALID_SOCKET;
    }

    static bool Init(std::string socketPath, int port) {
        path = socketPath;
        gLog::Info("checking if '%s' exists", path.c_str());

        DWORD fileAttr = GetFileAttributesA(path.c_str());
        if (fileAttr != INVALID_FILE_ATTRIBUTES) {
            // socket file exists already so try to delete
            if (!DeleteFileA(path.c_str())) {
                gLog::Error("Error deleting socket file");
                return false; // File failed to delete
            } else {
                gLog::Info("Successfully deleted socket file");
            }
        }

        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (result != 0) {
            gLog::Error("WSAStartup failed: %d", result);
            return false;
        }

        serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            gLog::Error("Error at socket(): %d", WSAGetLastError());
            WSACleanup();
            return false;
        }

        u_long mode = 1;  // Enable non-blocking mode
        result = ioctlsocket(serverSocket, FIONBIO, &mode);
        if (result == SOCKET_ERROR) {
            gLog::Error("ioctlsocket failed with error: %d", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        serverAddress.sun_family = AF_UNIX;
        strncpy(serverAddress.sun_path, socketPath.c_str(), sizeof(serverAddress.sun_path) - 1);

        result = bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
        if (result == SOCKET_ERROR) {
            gLog::Error("bind failed with error: %d", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        result = listen(serverSocket, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            gLog::Error("listen failed with error: %d", WSAGetLastError());
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        gLog::Info("Started server successfully");
        return true;
    }

    static void Close() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
            serverSocket = INVALID_SOCKET;
        }
        WSACleanup();

        gLog::Info("checking if '%s' exists", path.c_str());

        DWORD fileAttr = GetFileAttributesA(path.c_str());
        if (fileAttr != INVALID_FILE_ATTRIBUTES) {
            // socket file exists already so try to delete
            if (!DeleteFileA(path.c_str())) {
                gLog::Error("Error deleting socket file");
            } else {
                gLog::Info("Successfully deleted socket file");
            }
        }

        gLog::Info("Stopped server successfully");
    }

    static std::optional<std::string> Poll() {
        SOCKET clientSock = accept(serverSocket, nullptr, nullptr);
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

SOCKET gKernelServer::serverSocket;
struct sockaddr_un gKernelServer::serverAddress;
std::string gKernelServer::path;

#endif // GKERNELSERVER_H