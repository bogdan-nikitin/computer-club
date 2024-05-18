#ifndef COMPUTER_CLUB_H
#define COMPUTER_CLUB_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <string_view>
#include <functional>
#include <utility>

#include "time_util.h"
#include "util.h"

class computer_club {
private:
    inline static const std::string YOU_SHALL_NOT_PASS = "YouShallNotPass";
    inline static const std::string NOT_OPEN_YET = "NotOpenYet";
    inline static const std::string CLIENT_UNKNOWN = "ClientUnknown";
    inline static const std::string PLACE_IS_BUSY = "PlaceIsBusy";
    inline static const std::string I_CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";

    enum class event {
        INCOMING_CLIENT_CAME = 1,
        INCOMING_CLIENT_SAT = 2,
        INCOMING_CLIENT_WAITING = 3,
        INCOMING_CLIENT_LEFT = 4,
        OUTGOING_CLIENT_LEFT = 11,
        OUTGOING_CLIENT_SAT = 12,
        ERROR = 13
    };

    void print_event(event e);
    void print_time(time_util::time_t time);

    template<typename ... Args>
    void print(time_util::time_t time, event e, Args&& ... args) {
        print_time(time);
        output << ' ';
        print_event(e);
        ((output << ' ' << std::forward<Args>(args)), ...);
        output << '\n';
    }

    static constexpr time_util::time_t FREE_TABLE = -1;
    static constexpr std::size_t NO_TABLE = 0;

    struct table_info {
        time_util::time_t last_time = FREE_TABLE;
        time_util::time_t total_time = 0;
        std::size_t gain = 0;
    };

    std::queue<std::string_view> pending_clients;
    using clients_map_t = std::unordered_map<std::string, std::size_t, util::string_hash, std::equal_to<>>;
    clients_map_t clients;
    std::unordered_map<std::size_t, table_info> tables;
    std::size_t table_count;
    std::size_t busy_tables = 0;

    std::ostream& output;

    time_util::time_t open_time;
    time_util::time_t close_time;

    std::size_t hour_cost;

    void client_left_outgoing_event(time_util::time_t time, clients_map_t::iterator client_it);
    void client_sat_outgoing_event(time_util::time_t time, std::string_view client_name, std::size_t table);
    void error(time_util::time_t time, const std::string& message);

    void free_table(time_util::time_t time, table_info& table_info);
    void take_table(time_util::time_t time, clients_map_t::iterator client_it, std::size_t table);
public:
    void client_came(time_util::time_t time, std::string client_name);
    void client_sat(time_util::time_t time, const std::string& client_name, std::size_t table);
    void client_waiting(time_util::time_t time, const std::string& client_name);
    void client_left(time_util::time_t time, const std::string& client_name);

    computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::size_t table_count, std::ostream& output);
    void close();
    void read_event(std::istream& in);
};
#endif
