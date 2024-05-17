#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "computer_club.h"
#include "time_util.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: task.exe input_file\n";
        return EXIT_FAILURE;
    }
    std::ifstream in{argv[1]};
    std::size_t table_count;
    in >> table_count;
    time_util::time_t open_time = time_util::read_time(in);
    time_util::time_t close_time = time_util::read_time(in);
    std::size_t hour_cost;
    in >> hour_cost;
    std::stringstream out;
    computer_club computer_club{open_time, close_time, hour_cost, table_count, out};
    while (!in.eof()) {
        computer_club.read_event(in);
    }
    computer_club.close();
    std::cout << out.view();
}
