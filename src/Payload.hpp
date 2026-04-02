#pragma once

#include <string>

struct Date {
    int year;
    int month;
    int day;
};

struct Time {
    int hour;
    int minute;
};

struct Payload {
    std::string clientName;
    Date date;
    Time time;
};


