#include "computer_club.h"
#include "time_util.h"

computer_club::computer_club(time_util::time_t open_time, time_util::time_t close_time, std::size_t hour_cost, std::ostream& output)
 : open_time{open_time}, close_time{close_time}, hour_cost{hour_cost}, output{output} {}

 
 void computer_club::client_came(time_util::time_t time, std::string client_name) {
     if (time < open_time) {
         error(time, errors::NOT_OPEN_YET);
         return;
     }
     auto result = clients.try_emplace(std::move(client_name));
     if (!result.second) {
         error(time, errors::YOU_SHALL_NOT_PASS);
         return;
     }

 }
