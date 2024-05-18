#include <iostream>
#include <iomanip>
#include <cstdint>

#include "time_util.h"


time_util::time_t time_util::read_time(std::istream &in) {
    char delimiter;
    std::int16_t minutes;
    std::int16_t hours;
    in >> hours >> delimiter >> minutes;
    if (delimiter != ':' || hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
        return INVALID_TIME;
    }
    return as_time(hours, minutes);
} 

void time_util::print_time(time_util::time_t time, std::ostream& out) {
    out.fill('0');
    out << std::setw(2) << time / 60 << ':' << std::setw(2) << time % 60;
}

time_util::time_t time_util::as_time(std::uint8_t hours, std::uint8_t minutes) {
    return hours * 60 + minutes;
}

