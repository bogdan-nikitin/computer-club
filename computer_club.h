#include <iostream>
#include <string>
#include <queue>
#include <cstdint>
#include <unordered_map>
#include <string_view>


constexpr std::int16_t FREE_TABLE = -1;

class computer_club {
private:
    std::queue<std::string_view> pending_clients;
    std::unordered_map<std::string, std::pair<std::size_t, std::int16_t>> tables;

    std::ostream& output;

    std::uint16_t open_time;
    std::uint16_t close_time;

    std::size_t hour_cost;

    void client_left_outgoing_event(std::int16_t time, std::string_view name);
    void client_sat_outgoing_event(std::int16_t time, std::string_view name, std::size_t table_num);
    void error(std::int16_t time, const std::string& message);
public:
    void client_came(std::int16_t time, const std::string& client_name);
    void client_sat(std::uint16_t time, const std::string& client_name, std::size_t table_num);
    void client_waiting(std::uint16_t time, const std::string& client_name);
    void client_left(std::uint16_t time, const std::string& client_name);

    computer_club(std::uint16_t open_time, std::uint16_t close_time, std::size_t hour_cost, std::ostream& output);
};
