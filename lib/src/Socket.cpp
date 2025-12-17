#include "Socket.h"

#include <iostream>
#include <stdexcept>

int Socket::connect_to_server(std::string server_host, int server_port)
{

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        throw std::runtime_error("ERROR opening socket");
    }

    struct sockaddr_in server_address = create_server_address(server_host, server_port);
    int connection = connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (connection < 0)
    {
        throw std::runtime_error("ERROR connecting to address : " + server_host);
    }

    return socket_fd;
}

struct hostent *Socket::get_server(std::string host)
{
    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL)
    {
        throw std::runtime_error("ERROR, no such host: " + host);
    }

    return server;
}

struct sockaddr_in Socket::create_server_address(std::string host, int port)
{
    struct hostent *server = get_server(host.c_str());

    struct sockaddr_in server_address;
    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port);

    return server_address;
}

int Socket::write_to_server(char *buffer)
{
    // std::cout << "Writing bytes : " << strlen(buffer) << std::endl;
    int bytes = write(mSocketFd, buffer, strlen(buffer));
    if (bytes < 0)
    {
        throw std::runtime_error("ERROR writing to socket");
    }

    // std::cout << "Bytes written to server: " << bytes << std::endl;
    return bytes;
}

int Socket::read_response_from_server(void *buffer, size_t size)
{
    // std::cout << "read_response_from_server : Reading bytes: " << size << std::endl;
    bzero(buffer, size);
    int bytes = read(mSocketFd, buffer, size);

    // std::cout << "read_response_from_server : Bytes read from server: " << bytes << std::endl;
    if (bytes < 0)
    {
        throw std::runtime_error("ERROR reading from socket");
    }

    return bytes;
}

void Socket::write_to_server(uint8_t val)
{
    char buffer[sizeof(uint8_t) + 1];
    buffer[0] = static_cast<char>(val);
    buffer[1] = '\0';
    write_to_server(buffer);
}

void Socket::write_to_server(uint16_t val)
{
    char buffer[sizeof(uint16_t) + 1];
    memcpy(buffer, &val, sizeof(uint16_t));
    buffer[sizeof(uint16_t)] = '\0';
    write_to_server(buffer);
}

void Socket::write_to_server(uint32_t val)
{
    char buffer[sizeof(uint32_t) + 1];
    memcpy(buffer, &val, sizeof(uint32_t));
    buffer[sizeof(uint32_t)] = '\0';
    write_to_server(buffer);
}

void Socket::write_to_server(uint64_t val)
{
    char buffer[sizeof(uint64_t) + 1];
    memcpy(buffer, &val, sizeof(uint64_t));
    buffer[sizeof(uint64_t)] = '\0';
    write_to_server(buffer);
}

Socket::Socket(std::string hostname, int port)
{
    mServAddr = create_server_address(hostname, port);
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFd < 0)
    {
        throw std::runtime_error("ERROR opening socket");
    }

    int connection = connect(mSocketFd, (struct sockaddr *)&mServAddr, sizeof(mServAddr));
    if (connection < 0)
    {
        throw std::runtime_error("ERROR connecting to address : " + hostname);
    }
}

Socket::~Socket()
{
    close(mSocketFd);
}