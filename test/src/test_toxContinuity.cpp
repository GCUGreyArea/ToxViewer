#include <gtest/gtest.h>
#include <iostream>
#include <tox_client.h>

#include <string>

// A place to put toxes
typedef struct tox_desc_t {
    uint8_t code;
    uint32_t id;
    std::string message;
} tox_desc;

static tox_desc get_latest_tox() {
    Socket client("texttok.fzero.io", 4000);
    
    // Ask for tox ID
    uint8_t code = 1;
    client.write_to_server(code);

    // Read response
    int bytes = client.read_response_from_server((void*) &code,sizeof(uint8_t));

    // Get the values
    if(code != 64) {
        return {code, 0, "Error retrieving tox ID"};
    }

    uint32_t tox_id;

    bytes = client.read_response_from_server((void*) &tox_id, sizeof(tox_id)); 
    // ASSERT_EQ(bytes, sizeof(tox_id)) << "Failed to read full tox ID";
    
    // Ask for tox message
    code = 2;
    client.write_to_server(code);
    client.write_to_server(tox_id);


    // Pull the tox message
    uint32_t message_size;
    bytes = client.read_response_from_server(&code, sizeof(code));
    bytes = client.read_response_from_server(&message_size, sizeof(message_size));
    if(code != 65) {
        return {code, 0, "Error retrieving tox message"};
    }

    size_t size = static_cast<size_t>(message_size);
    char * message = new char[size + 1];
    message[message_size] = '\0';

    client.read_response_from_server(message, message_size);

    tox_desc desc = {code, tox_id, std::string(message, message_size)};
    delete [] message;

    return desc;
}


static void print_tox_desc(const tox_desc &desc) {
    std::cout << "Response ID: " << (int)desc.code << std::endl;
    std::cout << "Tox ID: " << (int)desc.id << std::endl;
    std::cout << "Message: \n" << desc.message << std::endl;
}

TEST(testToxContinuity,testThatToxCanBeAccessedAfterCall) {
    // constexpr const char * address = "texttok.fzero.io";
    // constexpr const int port = 4000;

    // // Conect to the server and pull a few tox
    
    // std::string addr = address;
    // Socket client(addr, port);

    tox_desc first_tox = get_latest_tox();
    EXPECT_EQ(first_tox.code, 65) << "Failed to get first tox";

    tox_desc second_tox = get_latest_tox();
    EXPECT_EQ(second_tox.code, 65) << "Failed to get second tox";

    tox_desc third_tox = get_latest_tox();
    EXPECT_EQ(third_tox.code, 65) << "Failed to get second tox";

    // Assert that we have individual tox IDs
    EXPECT_TRUE(first_tox.id != second_tox.id) << "Tox IDs match - first and second call";
    EXPECT_TRUE(third_tox.id != second_tox.id) << "Tox IDs match - third and first call";
    EXPECT_TRUE(first_tox.id != third_tox.id) << "Tox IDs match - first and third call";

    print_tox_desc(first_tox);
    print_tox_desc(second_tox);
    print_tox_desc(third_tox);
}