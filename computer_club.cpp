#include <type_traits>

#include "computer_club.h"
#include "time_util.h"

void computer_club::print_time(time_util::time_t time) {
    time_util::print_time(time, output);
}

void computer_club::print_event(event e) {
    output << static_cast<std::underlying_type_t<event>>(e);
}

computer_club::computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::size_t table_count, std::ostream& output)
    : open_time{open_time}, close_time{close_time}, hour_cost{hour_cost}, output{output}, table_count{table_count} {
    print_time(open_time);
    output << '\n';
}

 
 void computer_club::client_came(time_util::time_t time, std::string client_name) {
     if (time < open_time) {
         error(time, NOT_OPEN_YET);
         return;
     }
     auto result = clients.try_emplace(std::move(client_name));

     if (!result.second) {
         error(time, YOU_SHALL_NOT_PASS);
         return;
     }

     print_time(time);
     output << ' ';
     print_event(event::INCOMING_CLIENT_CAME);
     output << client_name << '\n';

 }


void computer_club::client_sat(time_util::time_t time, const std::string& client_name, std::size_t table_num) {
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }
    auto result = tables.try_emplace(table_num, client_it->first);
    if (!result.second) {
        error(time, PLACE_IS_BUSY);
        return;
    }
    print_time(time);
    output << ' ';
    print_event(event::INCOMING_CLIENT_SAT);
    output << ' ' << table_num << '\n';
}


void computer_club::client_waiting(time_util::time_t time, const std::string& client_name) {
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }
    if (pending_clients.empty()) {
        error(time, I_CAN_WAIT_NO_LONGER);
        return;
    }
    if (pending_clients.size() == table_count) {
        client_left_outgoing_event(time, client_name);
        return;
    }
    print_time(time);
    output << ' ';
    print_event(event::INCOMING_CLIENT_WAITING);
    output << ' ' << client_name << '\n';
    pending_clients.push(client_name);
}

void computer_club::client_left(time_util::time_t time, const std::string& client_name) {
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }
    print_time(time);
    output << ' ';
    print_event(event::INCOMING_CLIENT_LEFT);
    output << ' ' << client_name << '\n';

    auto table_info = client_it->second;

    clients.erase(client_it);

    auto table_time = table_info.time;
    if (table_time != NO_TABLE) {
        return;
    }
    auto table = table_info.table_num;
    tables.erase(table);
    if (pending_clients.empty()) {
        return;
    }
    auto next_client = pending_clients.front();
    pending_clients.pop();
    client_sat_outgoing_event(time, next_client, table);
}
