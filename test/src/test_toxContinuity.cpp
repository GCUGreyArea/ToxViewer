#include <gtest/gtest.h>
#include <iostream>
#include <tox_client.h>

#include <string>

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
    // Socket client(addr, port);

    Tox first_tox;
    get_tox(addr, port, first_tox);
    // EXPECT_EQ(first_tox.getCode(), 65) << "Failed to get first tox";

    Tox second_tox;
    get_tox(addr, port, second_tox);
    // EXPECT_EQ(second_tox.code, 65) << "Failed to get second tox";

    Tox third_tox;
    get_tox(addr, port, third_tox);
    // EXPECT_EQ(third_tox.code, 65) << "Failed to get second tox";

    // Assert that we have individual tox IDs - we know this test can fail!
    // EXPECT_FALSE(first_tox == second_tox) << "Tox IDs match - first and second call";
    // EXPECT_FALSE(third_tox == second_tox) << "Tox IDs match - third and first call";
    // EXPECT_FALSE(first_tox == third_tox) << "Tox IDs match - first and third call";

    // Print them out for visual inspection
    first_tox.print();
    second_tox.print();
    third_tox.print();

}