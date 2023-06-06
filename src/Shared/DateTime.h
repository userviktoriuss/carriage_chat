//
// Created by Viktor on 07.06.2023.
//

#pragma once

#include <string>

class DateTime {
public:
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;

    DateTime() {
        day = month = year = hour = minute = second = 0;
    }

    std::string ToString() {
        std::string date = std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year);
        std::string time = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
        return date + " " + time;
    }
};

bool operator<(const DateTime &dt1, const DateTime &dt2) {
    // TODO: todo =)
}
