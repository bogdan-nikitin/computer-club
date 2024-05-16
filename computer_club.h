#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <string_view>

#include "time_util.h"


class computer_club {
private:
    inline static const std::string YOU_SHALL_NOT_PASS = "YouShallNotPass";
    inline static const std::string NOT_OPEN_YET = "NotOpenYet";

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


    static constexpr time_util::time_t NO_TABLE = -1;

    struct table_info {
        std::size_t table_num;
        time_util::time_t time = NO_TABLE;
    };

    std::queue<std::string_view> pending_clients;
    std::unordered_map<std::string, table_info> clients;

    std::ostream& output;

    time_util::time_t open_time;
    time_util::time_t close_time;

    std::size_t hour_cost;

    void client_left_outgoing_event(time_util::time_t time, std::string_view name);
    void client_sat_outgoing_event(time_util::time_t time, std::string_view name, std::size_t table_num);
    void error(time_util::time_t time, const std::string& message);
public:
    void client_came(time_util::time_t time, std::string client_name);
    void client_sat(time_util::time_t time, const std::string& client_name, std::size_t table_num);
    void client_waiting(time_util::time_t time, const std::string& client_name);
    void client_left(time_util::time_t time, const std::string& client_name);

    computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::ostream& output);
};
