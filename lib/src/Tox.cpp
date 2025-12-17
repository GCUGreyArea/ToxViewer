#include "Tox.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>

Tox::Tox(Socket &client) {
    get_latest(client);
}

void Tox::get_latest(Socket & client) {
    // Ask for tox ID
    uint8_t code = 1;
    client.write_to_server(code);

    // Read response
    int bytes = client.read_response_from_server((void*) &code,sizeof(uint8_t));
    if(bytes < 0 || bytes != sizeof(uint8_t)) {
        throw std::runtime_error("Error reading tox code from server, bytes read: " + std::to_string(bytes));
    }

    // Insure the correct response code
    if(code != 64) {
        throw std::runtime_error("Error retrieving tox ID: " + std::to_string((int) code));
    }

    // Get the tox id
    uint32_t tox_id;
    bytes = client.read_response_from_server((void*) &tox_id, sizeof(tox_id)); 
    if(bytes < 0 || bytes != sizeof(tox_id)) {
        throw std::runtime_error("Error reading tox ID from server, bytes read: " + std::to_string(bytes));
    }    

    // Ask for tox message
    code = 2;
    client.write_to_server(code);
    client.write_to_server(tox_id);


    // Pull the tox message
    uint32_t message_size;
    bytes = client.read_response_from_server(&code, sizeof(code));
    if(bytes < 0 || bytes != sizeof(code)) {
        throw std::runtime_error("Error reading tox message code from server, bytes read: " + std::to_string(bytes));
    }
    else if(code != 65) {
        throw std::runtime_error("Error retrieving tox message code: " + std::to_string((int) code));
    }

    bytes = client.read_response_from_server(&message_size, sizeof(message_size));
    if(bytes < 0 || bytes != sizeof(message_size)) {
        throw std::runtime_error("Error reading tox message size from server, bytes read: " + std::to_string(bytes));
    }

    size_t size = static_cast<size_t>(message_size);
    char * message = new char[size + 1];
    message[message_size] = '\0';

    bytes = client.read_response_from_server(message, message_size);
    if(bytes < 0) {
        throw std::runtime_error("Error reading tox message from server,expected "  + std::to_string(size) + " bytes, read " + std::to_string(bytes) + " bytes");
    }
    else if(static_cast<size_t>(bytes) < size) {
        // Sometimes a tox is only partially read - read the rest
        // We assume that the behaviour only happens once per message - this could be wrong
        // A better solution would be a loop until all bytes are read, but this coould also lead to infinite looping 
        // if the server misbehaves...
        size_t newsize = message_size - static_cast<size_t>(bytes);
        bytes = client.read_response_from_server(message + bytes,newsize);
        if(bytes < 0 || static_cast<size_t>(bytes) != newsize) {
            throw std::runtime_error("Error reading tox message from server, expected "  + std::to_string(size) + " bytes, read " + std::to_string(bytes) + " bytes");
        }
    }

    // Assign the message values
    mId = tox_id;
    mMsg = std::string(message, message_size);
    delete [] message;
}


void Tox::pull(Socket & client) {
    // Ask for tox ID
    uint8_t code = 2;
    client.write_to_server(code);
    client.write_to_server(mId);

    // Read response
    int bytes = client.read_response_from_server((void*) &code,sizeof(uint8_t));
    if(bytes < 0 || bytes != sizeof(uint8_t)) {
        throw std::runtime_error("Error reading tox code from server, bytes read: " + std::to_string(bytes));
    }

    // Insure the correct response code
    if(code != 65) {
        throw std::runtime_error("Error retrieving tox ID: " + std::to_string((int) code));
    }

    // Pull the tox message
    uint32_t message_size;

    bytes = client.read_response_from_server(&message_size, sizeof(message_size));
    if(bytes < 0 || bytes != sizeof(message_size)) {
        throw std::runtime_error("Error reading tox message size from server, bytes read: " + std::to_string(bytes));
    }

    size_t size = static_cast<size_t>(message_size);
    char * message = new char[size + 1];
    message[message_size] = '\0';

    bytes = client.read_response_from_server(message, message_size);
    if(bytes < 0) {
        throw std::runtime_error("Error reading tox message from server,expected "  + std::to_string(size) + " bytes, read " + std::to_string(bytes) + " bytes");
    }
    else if(static_cast<size_t>(bytes) < size) {
        // Sometimes a tox is only partially read - read the rest
        // We assume that the behaviour only happens once per message - this could be wrong
        // A better solution would be a loop until all bytes are read, but this coould also lead to infinite looping 
        // if the server misbehaves...
        size_t newsize = message_size - static_cast<size_t>(bytes);
        bytes = client.read_response_from_server(message + bytes,newsize);
        if(bytes < 0 || static_cast<size_t>(bytes) != newsize) {
            throw std::runtime_error("Error reading tox message from server, expected "  + std::to_string(size) + " bytes, read " + std::to_string(bytes) + " bytes");
        }
    }

    // Assign the message values
    mMsg = std::string(message, message_size);
    delete [] message;
}

void Tox::print() {
    std::cout << "Message: \n" << mMsg << std::endl;
    std::cout << "Tox ID: " << std::hex << mId << std::dec << std::endl;
    std::cout << "Currently held Rating: " << mRating << std::endl;
}

bool Tox::operator==(const Tox &other) const {
    return (mId == other.mId) && (mMsg == other.mMsg);
}

void Tox::vote(Socket &client, bool upvote) {
    client.write_to_server(static_cast<uint8_t>(3)); // Vote code
    client.write_to_server(mId);
    if(upvote) {
        client.write_to_server(static_cast<uint8_t>(1)); // Upvote value
    }
    else {
        client.write_to_server(static_cast<uint8_t>(255));
    } // Downvote value
     std::cout << "Upvoted Tox ID: " << std::hex << mId << std::dec << std::endl;

    uint8_t code;
    int bytes = client.read_response_from_server(&code, sizeof(code));
    if(bytes < 0 || bytes != sizeof(code)) {
        throw std::runtime_error("Error reading upvote response from server, bytes read: " + std::to_string(bytes));
    }
    if(code != 66) {
        throw std::runtime_error("Error upvoting tox ID: " + id_as_hex());
    }

    // uint32_t rating;
    bytes = client.read_response_from_server(&mRating, sizeof(mRating));
    if(bytes < 0 || bytes != sizeof(mRating)) {
        throw std::runtime_error("Error reading upvote rating from server, bytes read: " + std::to_string(bytes));
    }

    pull(client); // Refresh the tox message after voting
}

std::string Tox::id_as_hex() {
    std::stringstream ss;
    ss << std::hex << mId;
    return ss.str();
}

void Tox::serialise(std::ostream & out) {
    out.write((char*) &mId,sizeof(mId));
    out.write((char*)&mRating,sizeof(mRating));

    size_t len = mMsg.length();
    out.write((char*)&len,sizeof(len));
    out.write(mMsg.data(),sizeof(char) * len);
}

void Tox::deserialise(std::istream & in) {
    in.read((char*)&mId,sizeof(mId));
    in.read((char*)&mRating,sizeof(mRating));

    size_t len = 0;
    in.read((char*)&len,sizeof(len));

    if(len == 0) {
        throw std::runtime_error("read of file returned 0");
    }
    char * msg = new char[len+1];
    if(msg == nullptr) {
        throw std::runtime_error("memory allocation failure");
    } 

    msg[len] = '\0';
    in.read(msg,len);

    mMsg = msg;
    delete [] msg;
}