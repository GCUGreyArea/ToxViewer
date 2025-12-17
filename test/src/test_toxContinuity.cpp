#include <gtest/gtest.h>
#include <iostream>
#include <tox_client.h>

#include <string>
#include <fstream>
#include <cstdlib>



// Maybe threading is messing up the socket connection? 
// For wahtev er erason in TEST we need to open a new connection fo reach tox retrieval
static void get_tox(std::string address, int port, Tox &tox) {
    Socket client(address, port);   
    tox.get_latest(client);
}

TEST(testToxContinuity,testThatToxCanBeAccessedAfterCall) {
    constexpr const char * address = "texttok.fzero.io";
    constexpr const int port = 4000;

    // Conect to the server and pull a few tox    
    std::string addr = address;

    Tox first_tox;
    EXPECT_NO_THROW(get_tox(addr, port, first_tox));

    Tox second_tox;
    EXPECT_NO_THROW(get_tox(addr, port, second_tox));

    Tox third_tox;
    EXPECT_NO_THROW(get_tox(addr, port, third_tox));

    // Print them out for visual inspection
    first_tox.print();
    second_tox.print();
    third_tox.print();
}


TEST(testToxContinuity,testThatToxCanBeSerialised) {
    constexpr const char * address = "texttok.fzero.io";
    constexpr const int port = 4000;

    // Conect to the server and pull a few tox    
    std::string addr = address;

    Tox first_tox;
    EXPECT_NO_THROW(get_tox(addr, port, first_tox));

    Tox second_tox;
    EXPECT_NO_THROW(get_tox(addr, port, second_tox));

    Tox third_tox;
    EXPECT_NO_THROW(get_tox(addr, port, third_tox));

    std::ofstream f;

    f.open("test.out",std::ios::out);

    first_tox.serialise(f);
    second_tox.serialise(f);
    third_tox.serialise(f);

    f.close();
    std::ifstream i;

    i.open("test.out",std::ios::in);

    Tox t1;
    Tox t2;
    Tox t3;
    t1.deserialise(i);
    t2.deserialise(i);
    t3.deserialise(i);

    ASSERT_TRUE(t1 == first_tox);
    ASSERT_TRUE(t2 == second_tox);
    ASSERT_TRUE(t3 == third_tox);

    // Remove the test file
    system("rm test.out");
}