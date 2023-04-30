//
// Created by Viktor on 30.04.2023.
//

#ifndef CARRIAGECHAT_CLIENTAPP_H
#define CARRIAGECHAT_CLIENTAPP_H

// ---------- Constants. ----------
#define MESSAGE_LENGTH 256
#define MAX_NICK_LENGTH 10

// ---------- Structures. ----------
struct user_data {
    char nickname[MAX_NICK_LENGTH];
};

struct message_data {
    user_data sender;
    char message[MESSAGE_LENGTH];
    // TODO: sending time
    // TODO: chat id
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

#endif //CARRIAGECHAT_CLIENTAPP_H
