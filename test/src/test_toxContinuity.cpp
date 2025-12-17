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

// Test to see if there are any hiden API calls that are not public 
// We could concievably create a program that increments the ID and 
// keeps trying, but this would be time consuming and could (potentiall)
// mess up the server???
static void test_api(std::string address, int port, Tox &tox) {
    Socket client(address, port);

    uint32_t id = tox.getId();

    uint8_t cmd = 4; // Test to see what we get
    client.write_to_server(cmd);
    client.write_to_server(id);

    cmd = 2;
    client.write_to_server(cmd);

    uint32_t code = 0;
    int bytes = client.read_response_from_server(&code,sizeof(code));
    if(bytes < 0) {
        throw std::runtime_error("Failed to read response");
    }
    else {
        std::cout << "Server returned " << code << std::endl; 
    }

    bytes = client.read_response_from_server(&code,sizeof(code));
    if(bytes < 0) {
        throw std::runtime_error("Failed to read response");
    }
    else {
        std::cout << "Server returned " << code << std::endl; 
    } 
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

TEST(testToxContinuity,testUnexposedAPIs) {
    constexpr const char * address = "texttok.fzero.io";
    constexpr const int port = 4000;

    // Conect to the server and pull a few tox    
    std::string addr = address;

    Tox first_tox;
    EXPECT_NO_THROW(get_tox(addr, port, first_tox));

    EXPECT_NO_THROW(test_api(addr, port, first_tox));
}