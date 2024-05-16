#include <type_traits>

#include "computer_club.h"
#include "time_util.h"


void computer_club::print_event(event e) {
    output << static_cast<std::underlying_type_t<event>>(e);
}

computer_club::computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::ostream& output)
 : open_time{open_time}, close_time{close_time}, hour_cost{hour_cost}, output{output} {}

 
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

     time_util::print_time(time, output);
     output << ' ';
     print_event(event::INCOMING_CLIENT_CAME);
     output << client_name << '\n';

 }
