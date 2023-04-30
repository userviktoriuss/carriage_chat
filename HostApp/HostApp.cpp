// HostApp.cpp
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

#define CLIENTS_QUEUE_MAX_SIZE 128
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MESSAGE_LENGTH 256

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
        // TODO: обработать сокет и вызвать тред
        //start_user_chat(new_connection, inService);
        //std::thread t_new_connection(start_user_chat, std::move(new_connection), std::move(inService));

        thds.push_back(std::thread(start_user_chat, std::move(new_connection), std::move(inService)));
    }

    for (auto& thr : thds) {
        thr.join();
    }
    closesocket(receive_connection_socket); // в случае удачного завершения операции возвращает нулевое значение.
    WSACleanup();
    return 0;
}


int load_library() {
    std::cout << "Started\n";
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    // Пытаемся подгрузить библиотеку
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0) {
        std::cout << "Startup failed with error: " << err << '\n';
        return 1;
    }

    // Проверяем, что загрузили нужную версию
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
    /*Если в момент вызова accept очередь пуста, функция не
возвращает управление до тех пор, пока с сервером не будет установлено хотя бы
одно соединение. В случае возникновения ошибки функция возвращает отрица
тельное значение.*/

    while (true) { // Waiting for client to write.
        char buf[MESSAGE_LENGTH];
        int iResult;

        iResult = recv(client_socket, buf, MESSAGE_LENGTH, 0);
        if (iResult <= 0) {  // клиент не отвечает
            std::cout << "recv failed: " << WSAGetLastError() << '\n';
            closesocket(client_socket);
            return;
        }
        
        std::cout << "[" << clientAddr.sin_addr.s_addr << ":" << clientAddr.sin_port << "]:  ";
        std::cout << buf << '\n';
    }
}

// TODO: план действий:
// разворачиваем хоста
// слушаем очередь подключений на connectionQueueSocket
// как получили запрос, отправляем его в свой поток

// внутри потока: получаем текст, возвращаем его в общий чат (оформить каждый чат 
// как отдельный объект и направлять сообщения в выбранный в данный момент пользователем)
// закрываем поток, когда юзер закрыл свой - как бы тут с безопасностью полегче было