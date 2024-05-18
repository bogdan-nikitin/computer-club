#include "computer_club.h"
#include "time_util.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>

enum class read_result {
    OK,
    FAIL,
    END,
};

static read_result read_line(std::istream& in, auto reader) {
    std::string line;
    std::getline(in, line);
    if (in.eof() || in.fail() || in.bad()) {
        std::cout << line << '\n';
        return read_result::FAIL;
    }
    if (line.empty()) {
        return read_result::END;
    }
    std::stringstream stream{line};
    if (!reader(stream) || stream.fail() || stream.bad()) {
        std::cout << line << '\n';
        return read_result::FAIL;
    }
    return read_result::OK;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: task.exe input_file\n";
        return EXIT_FAILURE;
    }
    std::ifstream in{argv[1]};
    if (in.bad() || in.fail()) {
        std::perror("Error opening file");
        return EXIT_FAILURE;
    }

    std::int32_t table_count;
    if (read_line(in, [&](auto& in) {
            in >> table_count;
            return table_count > 0;
        }) != read_result::OK) {
        return EXIT_FAILURE;
    };

    time_util::time_t open_time;
    time_util::time_t close_time;
    if (read_line(in, [&](auto& in) {
            open_time = time_util::read_time(in);
            close_time = time_util::read_time(in);
            return open_time != time_util::INVALID_TIME && close_time != time_util::INVALID_TIME &&
                   open_time <= close_time;
        }) != read_result::OK) {
        return EXIT_FAILURE;
    };
    std::int32_t hour_cost;
    if (read_line(in, [&](auto& in) {
            in >> hour_cost;
            return hour_cost > 0;
        }) != read_result::OK) {
        return EXIT_FAILURE;
    }
    std::stringstream out;
    computer_club computer_club{open_time, close_time, static_cast<std::size_t>(hour_cost),
                                static_cast<std::size_t>(table_count), out};
    time_util::time_t last_event = time_util::INVALID_TIME;
    while (true) {
        auto result = read_line(in, [&](auto& in) {
            last_event = computer_club.read_event(in, last_event);
            return last_event != time_util::INVALID_TIME;
        });
        if (result == read_result::END) {
            break;
        } else if (result == read_result::FAIL) {
            return EXIT_FAILURE;
        }
    }
    computer_club.close();
    std::cout << out.view();
}
