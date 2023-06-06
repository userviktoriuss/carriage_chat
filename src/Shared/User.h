//
// Created by Viktor on 07.06.2023.
//

#pragma once

#include <string>

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
