//
// Created by Viktor on 01.05.2023.
//

#pragma once

#include "../Shared/Message.h"
#include "../Shared/User.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <thread>

#define MAX_USERS_IN_CHAT 100
#define CLIENTS_QUEUE_MAX_SIZE 128
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MESSAGE_LENGTH 256

/*
 * Чат хранит информацию о себе и список всех пользователей
 *
 */

class Chat {
private:
    int chat_id_;
    std::unordered_map<int, User> members_; // Contains pairs (user_id, user)
    std::vector<Message> message_story_; // Contains the story of the dialog

public:
    explicit Chat(int chat_id) {
        chat_id_ = chat_id;
    }

    /// <summary>
    /// Adds user to chat.
    /// </summary>
    /// <param name="user">User to add.</param>
    /// <returns>true, if user added successfully, false otherwise.</returns>
    bool add_member(const User &user) {
        int user_id = user.get_id();
        if (members_.size() < MAX_USERS_IN_CHAT
            || members_.find(user_id) != members_.end()) {
            return false;
        }

        members_[user_id] = user;
        return true;
    }

    // returns true, if removed successfully
    bool remove_member(int user_id) {
        return members_.erase(user_id) == 1;
    }

    int get_id() const {
        return chat_id_;
    }

    void clear_story() {
        message_story_.clear();
    }

    std::vector<Message> fetch(DateTime date) {
        // binary search
        // assume that messages are sorted by date
        int left = 0; // last message before date
        int right = static_cast<int>(message_story_.size()); // first message >= date

        while (left + 1 < right) {
            int mid = (left + right) / 2;
            if (message_story_[mid].date < date) { // TODO: override operators for DateTime
                left = mid;
            } else {
                right = mid;
            }
        }
        return std::vector(message_story_.begin() + right, message_story_.end());
    }

    void add_message(Message message);
};

/*
class ChatManager {
private:
    std::vector<Chat> chats_;
public:
    std::vector<Message> fetch_chat()
};
*/

class Server {
private:
    int load_library() {
        std::cout << "Started initialization\n";
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

    static void start_user_chat(SOCKET client_socket, sockaddr_in clientAddr) {
        if (client_socket == INVALID_SOCKET) {
            std::cout << "Accept failed with error: " << WSAGetLastError() << '\n';
            closesocket(client_socket);
            return;
        }

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


public:
    int listen_connections() {
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

        int bind_status = bind(receive_connection_socket, (SOCKADDR *) &serverService, sizeof(serverService));
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
        while ((new_connection = accept(receive_connection_socket, (SOCKADDR *) &inService, NULL))) {
            thds.push_back(std::thread(start_user_chat, std::move(new_connection), std::move(inService)));
        }

        for (auto &thr: thds) {
            thr.join();
        }
        closesocket(receive_connection_socket); // ? ?????? ???????? ?????????? ???????? ?????????? ??????? ????????.
        WSACleanup();
        return 0;
    }
};
