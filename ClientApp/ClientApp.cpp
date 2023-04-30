// ClientApp.cpp

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MESSAGE_LENGTH 256
#define MAX_NICK_LENGTH 10

// ---------- Structures. ----------
struct user_data {
    char nickname[MAX_NICK_LENGTH];
};

struct message_data {
    user_data sender;
    char message[MESSAGE_LENGTH];
    // TODO: время отправки
    // TODO: конкретный чат
};

// ---------- Methods declaration. ----------
int load_library();

/// <summary>
/// Processes user authentification.
/// </summary>
/// <param name="data">Out parameter about user's account.</param>
/// <returns>0, if authentification went ok, error code otherwise.</returns>
int process_authentification(user_data& data) {
    std::cout << "Enter your nickname(not more than 9 non-space symbols): "; 
    std::cin >> data.nickname;
    return 0;
}

// --------- Methods defenition. ----------

int main()
{
    std::cout << "Welcome to chat client app.\n";

    int res = load_library();
    if (res != 0) return res;

    // Create socket.
    SOCKET connection_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_socket == INVALID_SOCKET) {
        std::cout << "Could not create socket\n";
        WSACleanup();
        return 1;
    }

    // Connect to host.
    sockaddr_in connection_addr;
    connection_addr.sin_family = AF_INET;
    connection_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    connection_addr.sin_port = htons(SERVER_PORT);
    int connection_status = connect(connection_socket, (SOCKADDR*)&connection_addr, sizeof(connection_addr));

    if (connection_status != 0) {
        std::cout << "Unable to connect to the host\n";
        closesocket(connection_socket);
        WSACleanup();
        return 1;
    }

    // Process authentification.
    user_data data;
    int auth_result = process_authentification(data);
    if (auth_result != 0) {
        std::cout << "Wrong login attempt";
        closesocket(connection_socket);
        WSACleanup();
        return 1;
    } // TODO: сериализовать структуру message и хранить в ней инфу про отправителя и текст

    std::string text;
    std::cin.get();
    std::getline(std::cin, text);
    text.resize(MESSAGE_LENGTH, '\0');
    while (text != "exit")
    {
        auto msg = text.c_str();
        send(connection_socket, msg, MESSAGE_LENGTH, 0); // такая херня, надо точно переписать
        std::getline(std::cin, text);
    }

    closesocket(connection_socket); // в случае удачного завершения операции возвращает нулевое значение.
    WSACleanup();
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
