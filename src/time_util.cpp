#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <string_view>
#include <charconv>

#include "time_util.h"

static time_util::time_t scan_part(const char *s) {
    time_util::time_t part;
    auto result = std::from_chars(s, s + 2, part);
    if (result.ec == std::errc::invalid_argument) {
        return time_util::INVALID_TIME;
    }
    return part;
}


time_util::time_t time_util::read_time(std::istream &in) {
    char delimiter;
    std::string time;
    in >> time;
    if (time.size() != 5 || time[2] != ':') {
        return INVALID_TIME;
    }
    std::int16_t hours = scan_part(time.data());
    std::int16_t minutes = scan_part(time.data() + 3);
    if (hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60) {
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

