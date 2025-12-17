#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <string>

class Socket {
public:
    Socket(std::string hostname, int port);
    ~Socket();

    void write_to_server(uint8_t val);
    void write_to_server(uint16_t val);
    void write_to_server(uint32_t val);
    void write_to_server(uint64_t val);
    int write_to_server(char *buffer);

    int read_response_from_server(void *buffer, size_t size);
  
protected:
    int connect_to_server(std::string server_host, int server_port);
    struct hostent* get_server(std::string host);
    struct sockaddr_in create_server_address(std::string host, int port);
    

private:
    struct sockaddr_in mServAddr;
    int mSocketFd;
};

#endif // SOCKET_H