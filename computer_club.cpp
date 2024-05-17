#include <type_traits>

#include "computer_club.h"
#include "time_util.h"

static const std::size_t MINUTES_IN_HOUR = 60;

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

    auto &table_info = tables[table_num];
    if (table_info.last_time != FREE_TABLE) {
        error(time, PLACE_IS_BUSY);
        return;
    }

    table_info.last_time = time;

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
        client_left_outgoing_event(time, client_it);
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

    auto table = client_it->second;
    if (table == NO_TABLE) {
        return;
    }

    clients.erase(client_it);

    auto &table_info = tables[table];
    if (table_info.last_time == FREE_TABLE) {
        return;
    }
    free_table(time, table_info);
    if (pending_clients.empty()) {
        return;
    }
    auto next_client = pending_clients.front();
    pending_clients.pop();
    client_sat_outgoing_event(time, next_client, table);
}


void computer_club::client_left_outgoing_event(time_util::time_t time, std::unordered_map<std::string, std::size_t>::iterator client_it) {
    print_time(time);
    output << ' ';
    print_event(event::OUTGOING_CLIENT_LEFT);
    output << ' ' << client_it->first << '\n';

    auto table = client_it->second;
    auto &table_info = tables[table];
    free_table(time, table_info);
}

void computer_club::client_sat_outgoing_event(time_util::time_t time, std::string_view name, std::size_t table_num) {
    auto client_it = clients.find(name);
    client_it->second = table_num;
    
    tables[table_num].last_time = time;

    print_time(time);
    output << ' ';
    print_event(event::OUTGOING_CLIENT_SAT);
    output << ' ' << table_num << '\n';
}

void computer_club::error(time_util::time_t time, const std::string& message) {
    print_time(time);
    output << ' ';
    print_event(event::ERROR);
    output << message << '\n';
}

void computer_club::free_table(time_util::time_t time, table_info& table_info) {
    time_util::time_t time_delta = time - table_info.last_time;
    table_info.gain += (time_delta + MINUTES_IN_HOUR - 1) / MINUTES_IN_HOUR;
    table_info.total_time += time_delta;
    table_info.last_time = FREE_TABLE;
}
