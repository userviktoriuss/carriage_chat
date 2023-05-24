#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "HostApp.h"
#include <thread>
#include <vector>


int load_library();
void start_user_chat(SOCKET client_socket, sockaddr_in clientAddr);

int main()
{
    std::cout << "Welcome to chat host app.\n";

    int res = load_library();
    if (res != 0) return res;

    // Create socket.
    SOCKET receive_connection_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (receive_connection_socket == INVALID_SOCKET) {
        std::cout << "Could not create socket\n";
        WSACleanup();
        return 1;
    }

    // Create TCP-host.
    sockaddr_in serverService;
    serverService.sin_family = AF_INET;
    serverService.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverService.sin_port = htons(SERVER_PORT);

    int bind_status = bind(receive_connection_socket, (SOCKADDR*)&serverService, sizeof(serverService));
    if (bind_status != 0) {
        std::cout << "Unable to bind socket to local address\n";
        WSACleanup();
        closesocket(receive_connection_socket);
        return 1;
    }

    // Start listening for connections.
    listen(receive_connection_socket, CLIENTS_QUEUE_MAX_SIZE);

    std::vector<std::thread> thds;
    sockaddr_in inService;
    SOCKET new_connection;
    while ((new_connection = accept(receive_connection_socket, (SOCKADDR*)&inService, NULL))) {
        // TODO: ?????????? ????? ? ??????? ????
        //start_user_chat(new_connection, inService);
        //std::thread t_new_connection(start_user_chat, std::move(new_connection), std::move(inService));

        thds.push_back(std::thread(start_user_chat, std::move(new_connection), std::move(inService)));
    }

    for (auto& thr : thds) {
        thr.join();
    }
    closesocket(receive_connection_socket); // ? ?????? ???????? ?????????? ???????? ?????????? ??????? ????????.
    WSACleanup();
    return 0;
}


int load_library() {
    std::cout << "Started\n";
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // ???????? ?????????? ??????????
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0) {
        std::cout << "Startup failed with error: " << err << '\n';
        return 1;
    }

    // ?????????, ??? ????????? ?????? ??????
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        std::cout << "Could not find the required version of Winsock.dll\n";
        WSACleanup();
        return 1;
    }
    std::cout << "Initialized library successfuly\n";
    return 0;
}

void start_user_chat(SOCKET client_socket, sockaddr_in clientAddr) {
    if (client_socket == INVALID_SOCKET) {
        std::cout << "Accept failed with error: " << WSAGetLastError() << '\n';
        closesocket(client_socket);
        return;
    }
    /*???? ? ?????? ?????? accept ??????? ?????, ??????? ??
?????????? ?????????? ?? ??? ???, ???? ? ???????? ?? ????? ??????????? ???? ??
???? ??????????. ? ?????? ????????????? ?????? ??????? ?????????? ??????
??????? ????????.*/

    while (true) { // Waiting for client to write.
        char buf[MESSAGE_LENGTH];
        int iResult;

        iResult = recv(client_socket, buf, MESSAGE_LENGTH, 0);
        if (iResult <= 0) {  // ?????? ?? ????????
            std::cout << "recv failed: " << WSAGetLastError() << '\n';
            closesocket(client_socket);
            return;
        }
        
        std::cout << "[" << clientAddr.sin_addr.s_addr << ":" << clientAddr.sin_port << "]:  ";
        std::cout << buf << '\n';
    }
}

// TODO: ???? ????????:
// ????????????? ?????
// ??????? ??????? ??????????? ?? connectionQueueSocket
// ??? ???????? ??????, ?????????? ??? ? ???? ?????

// ?????? ??????: ???????? ?????, ?????????? ??? ? ????? ??? (???????? ?????? ??? 
// ??? ????????? ?????? ? ?????????? ????????? ? ????????? ? ?????? ?????? ?????????????)
// ????????? ?????, ????? ???? ?????? ???? - ??? ?? ??? ? ????????????? ??????? ????