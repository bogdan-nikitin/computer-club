#include "computer_club.h"
#include "time_util.h"

using namespace time_util;

#include <sstream>

#include <gtest/gtest.h>

TEST(correctness, example) {
    std::stringstream output;
    computer_club computer_club{
        as_time(9, 0), 
        as_time(19, 0),
        10,
        3,
        output
    };
    computer_club.client_came(as_time(8, 48), "client1");
    computer_club.client_came(as_time(9, 41), "client1");
    computer_club.client_came(as_time(9, 48), "client2");
    computer_club.client_waiting(as_time(9, 52), "client1");
    computer_club.client_sat(as_time(9, 54), "client1", 1);
    computer_club.client_sat(as_time(10, 25), "client2", 2);
    computer_club.client_came(as_time(10, 58), "client3");
    computer_club.client_sat(as_time(10, 59), "client3", 3);
    computer_club.client_came(as_time(11, 30), "client4");
    computer_club.client_sat(as_time(11, 35), "client4", 2);
    computer_club.client_waiting(as_time(11, 45), "client4");
    computer_club.client_left(as_time(12, 33), "client1");
    computer_club.client_left(as_time(12, 43), "client2");
    computer_club.client_left(as_time(15, 52), "client4");
    computer_club.close();
    EXPECT_EQ(
"09:00\n"
"08:48 1 client1\n"
"08:48 13 NotOpenYet\n"
"09:41 1 client1\n"
"09:48 1 client2\n"
"09:52 3 client1\n"
"09:52 13 ICanWaitNoLonger!\n"
"09:54 2 client1 1\n"
"10:25 2 client2 2\n"
"10:58 1 client3\n"
"10:59 2 client3 3\n"
"11:30 1 client4\n"
"11:35 2 client4 2\n"
"11:35 13 PlaceIsBusy\n"
"11:45 3 client4\n"
"12:33 4 client1\n"
"12:33 12 client4 1\n"
"12:43 4 client2\n"
"15:52 4 client4\n"
"19:00 11 client3\n"
"19:00\n"
"1 70 05:58\n"
"2 30 02:18\n"
"3 90 08:01\n",
    output.view());
}

