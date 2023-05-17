#include "../util.c"
#include "../server.c"
#include "../protocol.c"
#include "../crypto.c"

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    char* string;
    char* compressed;
    compress_string(string, compressed);

    /*char* example = "GET 912ec803b2ce49e4a541068d495ab570 YARN/1.0\r\nORIGIN:127.0.0.1:9000\r\nPORT:5000\r\n"; 
    yarn_packet_t* packet = to_packet(example);
    
    
    YARN_SERVER* yarn = yarn_easy_init(stdout, 10, 9001);
    
    while (1) {
        printf("Waiting for connections...\n");

        int cli_sock = 0;
        size_t addr_size = 0;
        struct sockaddr_in client;

        addr_size = sizeof(struct sockaddr_in);
        herror(stdout, (cli_sock = accept(yarn->socket, (struct sockaddr*)&client, (socklen_t*)&addr_size)), "Failed to accept connection");
        
        printf("Connection established...!\n");
    }

    yarn_close(yarn);
    */
}
