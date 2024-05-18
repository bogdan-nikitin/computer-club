#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <tuple>

#include "computer_club.h"
#include "time_util.h"

bool read_line(std::istream& in, auto reader) {
    std::string line;
    std::getline(in, line);
    if (in.eof() || in.fail() || in.bad()) {
        std::cout << "IN error\n";
        std::cout << line << '\n';
        return false;
    }
    std::stringstream stream{line};
    if (!reader(stream) || !stream.eof() || stream.fail() || stream.bad()) {
        std::cout << line << '\n';
        return false;
    }
    return true;
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: task.exe input_file\n";
        return EXIT_FAILURE;
    }
    std::ifstream in{argv[1]};

    std::size_t table_count;
    if (!read_line(in, [&](auto &in) {
            in >> table_count;
            return true;
        })) {
        return EXIT_FAILURE;
    };

    time_util::time_t open_time;
    time_util::time_t close_time;
    if (!read_line(in, [&](auto &in) {
        open_time = time_util::read_time(in);
        close_time = time_util::read_time(in);
            return open_time != time_util::INVALID_TIME && close_time != time_util::INVALID_TIME;
        })) {
        return EXIT_FAILURE;
    };
    std::size_t hour_cost;
    if (!read_line(in, [&](auto &in) {
                in >> hour_cost;
            return true;
                })) {
        return EXIT_FAILURE;
    }
    std::stringstream out;
    computer_club computer_club{open_time, close_time, hour_cost, table_count, out};
    bool has_input = true;
    while (has_input) {
        computer_club::read_result result;
        if (!read_line(in, [&](auto &in) {
            result = computer_club.read_event(in);
            return result == computer_club::read_result::OK;
        })) {
            return EXIT_FAILURE;
        }
        switch (result) {
        case computer_club::read_result::OK:
            break;
        case computer_club::read_result::FAIL:
            return EXIT_FAILURE;
            break;
        case computer_club::read_result::ERROR:
            return EXIT_FAILURE;
            break;
        case computer_club::read_result::END:
            has_input = false;
            break;
        }
    }
    computer_club.close();
    std::cout << out.view();
}
