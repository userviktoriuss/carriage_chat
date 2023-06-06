#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "HostApp.h"
#include <thread>
#include <vector>

void start_user_chat(SOCKET client_socket, sockaddr_in clientAddr);

int main() {
    std::cout << "Welcome to chat host app.\n";
    Server server = Server();

    server.listen_connections();

    return 0;
}

