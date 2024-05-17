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


    static constexpr time_util::time_t FREE_TABLE = -1;
    static constexpr std::size_t NO_TABLE = 0;

    struct table_info {
        time_util::time_t last_time = FREE_TABLE;
        time_util::time_t total_time = 0;
        std::size_t gain = 0;
    };

    std::queue<std::string_view> pending_clients;
    std::unordered_map<std::string, std::size_t> clients;
    std::unordered_map<std::size_t, table_info> tables;
    std::size_t table_count;

    std::ostream& output;

    time_util::time_t open_time;
    time_util::time_t close_time;

    std::size_t hour_cost;

    void client_left_outgoing_event(time_util::time_t time, std::unordered_map<std::string, std::size_t>::iterator client_it);
    void client_sat_outgoing_event(time_util::time_t time, std::string_view name, std::size_t table_num);
    void error(time_util::time_t time, const std::string& message);

    void free_table(time_util::time_t time, table_info& table_info);
public:
    void client_came(time_util::time_t time, std::string client_name);
    void client_sat(time_util::time_t time, const std::string& client_name, std::size_t table_num);
    void client_waiting(time_util::time_t time, const std::string& client_name);
    void client_left(time_util::time_t time, const std::string& client_name);

    computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::size_t table_count, std::ostream& output);
};
