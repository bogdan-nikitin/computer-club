#include <iostream>
#include <iomanip>

#include "time_util.h"


time_util::time_t time_util::read_time(std::istream &in) {
    char delimiter;
    std::int16_t minutes;
    std::int16_t hours;
    in >> hours >> delimiter >> minutes;
    return minutes + hours * 60;
} 

void time_util::print_time(time_util::time_t time, std::ostream& out) {
    out.fill('0');
    out << std::setw(2) << time / 60 << ':' << std::setw(2) << time % 60;
}


