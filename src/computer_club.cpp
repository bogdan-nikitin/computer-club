#include <type_traits>
#include <vector>
#include <ranges>
#include <iostream>
#include <utility>
#include <string_view>

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
     print(time, event::INCOMING_CLIENT_CAME, client_name);
     if (time < open_time) {
         error(time, NOT_OPEN_YET);
         return;
     }
     auto result = clients.try_emplace(std::move(client_name));

     if (!result.second) {
         error(time, YOU_SHALL_NOT_PASS);
         return;
     }
 }


void computer_club::client_sat(time_util::time_t time, const std::string& client_name, std::size_t table) {
    print(time, event::INCOMING_CLIENT_SAT, client_name, table);
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }

    auto &table_info = tables[table];
    if (table_info.last_time != FREE_TABLE) {
        error(time, PLACE_IS_BUSY);
        return;
    }

    take_table(time, client_it, table);
    ++busy_tables;
}


void computer_club::client_waiting(time_util::time_t time, const std::string& client_name) {
    print(time, event::INCOMING_CLIENT_WAITING, client_name);
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }
    if (busy_tables < table_count) {
        error(time, I_CAN_WAIT_NO_LONGER);
        return;
    }
    if (pending_clients.size() == table_count) {
        client_left_outgoing_event(time, client_it);
        return;
    }
    pending_clients.push(client_name);
}

void computer_club::client_left(time_util::time_t time, const std::string& client_name) {
    print(time, event::INCOMING_CLIENT_LEFT, client_name);
    auto client_it = clients.find(client_name);
    if (client_it == clients.end()) {
        error(time, CLIENT_UNKNOWN);
        return;
    }

    auto table = client_it->second;
    if (table == NO_TABLE) {
        return;
    }

    clients.erase(client_it);

    auto &table_info = tables[table];
    free_table(time, table_info);
    if (pending_clients.empty()) {
        return;
    }
    auto next_client = pending_clients.front();
    pending_clients.pop();
    client_sat_outgoing_event(time, next_client, table);
}


void computer_club::client_left_outgoing_event(time_util::time_t time, clients_map_t::iterator client_it) {
    print(time, event::OUTGOING_CLIENT_LEFT, client_it->first);

    auto table = client_it->second;
    auto &table_info = tables[table];
    free_table(time, table_info);
}

void computer_club::client_sat_outgoing_event(time_util::time_t time, const std::string& client_name, std::size_t table) {
    auto client_it = clients.find(client_name);

    take_table(time, client_it, table);

    print(time, event::OUTGOING_CLIENT_SAT, client_it->first, table);
}

void computer_club::error(time_util::time_t time, const std::string& message) {
    print(time, event::ERROR, message);
}

void computer_club::free_table(time_util::time_t time, table_info& table_info) {
    time_util::time_t time_delta = time - table_info.last_time;
    table_info.gain += (time_delta + MINUTES_IN_HOUR - 1) / MINUTES_IN_HOUR * hour_cost;
    table_info.total_time += time_delta;
    table_info.last_time = FREE_TABLE;
    --busy_tables;
}

void computer_club::close() {
    auto keys = std::views::keys(clients);
    std::vector<std::string_view> clients_ordered{keys.begin(), keys.end()};
    std::ranges::sort(clients_ordered);
    for (auto name : clients_ordered) {
        client_left_outgoing_event(close_time, clients.find(name));
    }
    print_time(close_time);
    output << '\n';
    for (std::size_t i = 1; i <= table_count; ++i) {
        auto table_info = tables[i];
        output << i << ' ' << table_info.gain << ' ';
        print_time(table_info.total_time);
        output << '\n';
    }
}

void computer_club::read_event(std::istream& in) {
    time_util::time_t time = time_util::read_time(in);
    std::size_t id;
    in >> id;
    if (in.eof()) {
        return;
    }
    std::string client_name;
    in >> client_name;
    switch (id) {
        case 1: 
            client_came(time, std::move(client_name));
            break;
        case 2:
            std::size_t table;
            in >> table;
            client_sat(time, client_name, table);
            break;
        case 3:
            client_waiting(time, client_name);
            break;
        case 4:
            client_left(time, client_name);
            break;
    }
}


void computer_club::take_table(time_util::time_t time, clients_map_t::iterator client_it, std::size_t table) {
    client_it->second = table;
    tables[table].last_time = time;
}
