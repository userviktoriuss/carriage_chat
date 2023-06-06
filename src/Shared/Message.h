//
// Created by Viktor on 06.06.2023.
//

#pragma once

#include <string>
#include "string.h"
#include "DateTime.h"

class Message {
public:
    int sender_id;
    DateTime date;
    std::string text;

    // Возвращает пару (указатель на строку, кол-во символов) - сериализованные байты
    std::pair<char *, size_t> Serialize() {
        size_t size = sizeof(int) + sizeof(DateTime) + text.size();
        char *ser = new char[size];

        strncpy(ser, (char *) &sender_id, sizeof(int));

        for (int i = 0; i < sizeof(DateTime); ++i) {
            ser[sizeof(int) + i] = *(((char *) &date) + i);
        }

        strncpy(ser + sizeof(int) + sizeof(DateTime), text.c_str(), text.size());
        return std::make_pair(ser, size);
    }

    void Deserialize(char *&src, size_t count) {
        sender_id = *((int *) src);
        date = *((DateTime *) (src + sizeof(int)));
        size_t prefix_size = sizeof(int) + sizeof(DateTime);
        text = std::string(src + prefix_size, count - prefix_size);
    }
};

