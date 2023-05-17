#ifndef _YARNSERVER_H
#define _YARNSERVER_H

#include <sys/socket.h>
#include <stdint.h>

typedef struct yarn_server { 
    int socket;
    uint16_t port;
    int backlog;
    struct sockaddr_in server_addr;
} YARN_SERVER;

#endif
