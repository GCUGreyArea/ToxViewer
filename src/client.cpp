#include "client.h"

#include <stdexcept>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>

typedef struct __attribute__((packed)) tox_req_t {
    uint8_t code;
    uint64_t tox_id;
} tox_req;

#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))

int main() {
    Socket client("texttok.fzero.io", 4000);

    // Ask for tox ID
    uint8_t code = 1;
    client.write_to_server(code);

    // Read response
    int bytes = client.read_response_from_server((void*) &code,sizeof(uint8_t));

    // Get the values
    if(code != 64) {
        std::cerr << "Error response from server, ID: " << (int) code << std::endl;
        return 1;
    }

    uint32_t tox_id;

    bytes = client.read_response_from_server((void*) &tox_id, sizeof(tox_id)); 
    
    std::cout << "Response ID: " << (int) code << std::endl;
    std::cout << "Tox ID: " << (int) tox_id << std::endl;
    


    std::cout << "Requesting message for Tox ID: " << (int) tox_id << std::endl;


    // Ask for tox message
    code = 2;
    client.write_to_server(code);
    client.write_to_server(tox_id);


    uint32_t message_size;
    bytes = client.read_response_from_server(&code, sizeof(code));
    bytes = client.read_response_from_server(&message_size, sizeof(message_size));
    if(code != 65) {
        std::cerr << "Error response from server, code: " << (int)code << std::endl;
        return 1;
    }

    std::cout << "Response ID: " << (int)code << std::endl;
    std::cout << "Message size: " << (int)message_size << std::endl;

    std::cout << "Message size to read: " <<(int) message_size << std::endl;

    size_t size = static_cast<size_t>(message_size);
    char * message = new char[size + 1];
    message[message_size] = '\0';

    client.read_response_from_server(message, message_size);
    // memcpy(message, buff, size);

    std::cout << "Message size: " << message_size << std::endl;
    std::cout << "Message: " << std::string(message, message_size) << std::endl;

    delete [] message;

    return 0;
}