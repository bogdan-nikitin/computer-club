#include <iostream>
#include <cstdint>

#include "computer_club.h"


std::int16_t read_time(std::istream& in) {
    char delimiter;
    std::int16_t minutes;
    std::int16_t hours;
    in >> hours >> delimiter >> minutes;
    return minutes + hours * 60;
} 


int main() {
    std::size_t table_count;
    std::cin >> table_count;

}
