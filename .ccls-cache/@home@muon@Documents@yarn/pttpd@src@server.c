#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util.c"
#include "server.h"

YARN_SERVER* yarn_easy_init(FILE* log, int backlog, int port) {
    YARN_SERVER* yarn = malloc(sizeof(YARN_SERVER));

    yarn->socket = herror(log, socket(AF_INET, SOCK_STREAM, 0),
            "Unable to create socket");
    
    yarn->backlog = backlog;
    yarn->port = htons(port);

    yarn->server_addr.sin_family = AF_INET;
    yarn->server_addr.sin_addr.s_addr = INADDR_ANY;
    yarn->server_addr.sin_port = yarn->port;
    
    herror(log, bind(yarn->socket,(struct sockaddr*)&yarn->server_addr, sizeof(yarn->server_addr)),
            "Unable to bind socket");
    herror(log, listen(yarn->socket, backlog),
    "Unable to listen()"); 

    return yarn;
}

int yarn_handle(int client) {
    size_t bufsize = 1024;
    char* buffer = malloc(bufsize);
    size_t cnt, received = 0;

    while ((cnt = read(client, buffer+received*sizeof(char), bufsize-received-1)) > 0) {
        received += cnt;
        if (received > bufsize-1 || strncmp(buffer+(received-5)*sizeof(char), "\r\n\r\n", 4) == 0) {
            break;
        }
    }
    
    free(buffer);
    return received;
}

void yarn_close(YARN_SERVER* yarn) {
    free(yarn);
}
