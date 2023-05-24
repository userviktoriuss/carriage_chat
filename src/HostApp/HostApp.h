//
// Created by Viktor on 01.05.2023.
//

#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#define MAX_USERS_IN_CHAT 100
#define CLIENTS_QUEUE_MAX_SIZE 128
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define MESSAGE_LENGTH 256

/*
 * Чат хранит информацию о себе и список всех пользователей
 *
 */

class DateTime {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
};

bool operator<(const DateTime& dt1, const DateTime& dt2) {
    // TODO: todo =)
}

struct Message {
    int sender_id;
    std::string text;
    DateTime date;
};

class User {
private:
    int user_id_;
    std::string username_;
public:
    [[nodiscard]] int get_id() const {
        return user_id_;
    }

    [[nodiscard]] std::string get_username() const {
        return username_;
    }
};

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
    void load_library();

public:
    void listen_connections();

    void listen_messages();
};
