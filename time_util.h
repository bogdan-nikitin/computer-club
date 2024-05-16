#ifndef TIME_UTIL_H
#define TIME_UTIL_H

#include <cstdint>
#include <iostream>


namespace time_util {
    using time_t = std::int16_t;
    time_t read_time(std::istream& in);
    void print_time(time_t time, std::ostream& out);
}

#endif
