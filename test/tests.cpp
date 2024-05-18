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
    EXPECT_EQ(std::string_view{
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
"3 90 08:01\n"},
    output.view());
}

TEST(correctness, sort) {
    std::stringstream output;
    computer_club computer_club{
        as_time(10, 0),
        as_time(12, 0),
        1,
        12,
        output
    };

    computer_club.client_came(as_time(11, 0), "x");
    computer_club.client_sat(as_time(11, 1), "x", 1);

    computer_club.client_came(as_time(11, 2), "a");
    computer_club.client_sat(as_time(11, 3), "a", 2);

    computer_club.client_came(as_time(11, 4), "b");
    computer_club.client_sat(as_time(11, 5), "b", 3);

    computer_club.client_came(as_time(11, 6), "c");
    computer_club.client_sat(as_time(11, 7), "c", 4);

    computer_club.client_came(as_time(11, 8), "y");
    computer_club.client_sat(as_time(11, 9), "y", 5);

    computer_club.client_came(as_time(11, 10), "f");
    computer_club.client_sat(as_time(11, 11), "f", 6);

    computer_club.client_came(as_time(11, 12), "e");
    computer_club.client_sat(as_time(11, 13), "e", 7);

    computer_club.client_came(as_time(11, 14), "d");
    computer_club.client_sat(as_time(11, 15), "d", 8);

    computer_club.client_came(as_time(11, 16), "zac");
    computer_club.client_sat(as_time(11, 17), "zac", 9);

    computer_club.client_came(as_time(11, 18), "zab");
    computer_club.client_sat(as_time(11, 19), "zab", 10);

    computer_club.client_came(as_time(11, 20), "zaa");
    computer_club.client_sat(as_time(11, 21), "zaa", 11);

    computer_club.client_came(as_time(11, 22), "z");
    computer_club.client_sat(as_time(11, 23), "z", 12);

    computer_club.close();
    EXPECT_EQ(
            std::string_view{
"10:00\n"

"11:00 1 x\n"
"11:01 2 x 1\n"

"11:02 1 a\n"
"11:03 2 a 2\n"

"11:04 1 b\n"
"11:05 2 b 3\n"

"11:06 1 c\n"
"11:07 2 c 4\n"

"11:08 1 y\n"
"11:09 2 y 5\n"

"11:10 1 f\n"
"11:11 2 f 6\n"

"11:12 1 e\n"
"11:13 2 e 7\n"

"11:14 1 d\n"
"11:15 2 d 8\n"

"11:16 1 zac\n"
"11:17 2 zac 9\n"

"11:18 1 zab\n"
"11:19 2 zab 10\n"

"11:20 1 zaa\n"
"11:21 2 zaa 11\n"

"11:22 1 z\n"
"11:23 2 z 12\n"

"12:00 11 a\n"
"12:00 11 b\n"
"12:00 11 c\n"
"12:00 11 d\n"
"12:00 11 e\n"
"12:00 11 f\n"
"12:00 11 x\n"
"12:00 11 y\n"
"12:00 11 z\n"
"12:00 11 zaa\n"
"12:00 11 zab\n"
"12:00 11 zac\n"

"12:00\n"
"1 1 00:59\n"
"2 1 00:57\n"
"3 1 00:55\n"
"4 1 00:53\n"
"5 1 00:51\n"
"6 1 00:49\n"
"7 1 00:47\n"
"8 1 00:45\n"
"9 1 00:43\n"
"10 1 00:41\n"
"11 1 00:39\n"
"12 1 00:37\n"}
            , output.view());
}

