#ifndef _YARNPROTOCOL_H
#define _YARNPROTOCOL_H

#include <stdlib.h>

#define MAX_LENGTH 64

// a single header (linked list element) 
typedef struct Header {
    struct Header* next;
    char* key;
    char* value;
} yarn_header_t;

// yarn_headermap_t is a linked list storing key value pairs of headers
typedef struct HeaderMap {
    yarn_header_t* head;
    yarn_header_t* tail;
    int length;
} yarn_headermap_t;

// create a new map
yarn_headermap_t* map();
// add a k-v-pair to the map
yarn_headermap_t* add(yarn_headermap_t*, char*, char*);
// remove all elements matching the specified key
yarn_headermap_t* remove_key(yarn_headermap_t*, char*);
// free an entire headermap and all headers
void free_map(yarn_headermap_t*);

// an easy-to-use packet structure
typedef struct yarn_packet {
    char*   method;
    char*   target;
    char*   version;

    yarn_headermap_t* headers;
} yarn_packet_t;

// free the memory of a packet (includes all the string magic)
void free_packet(yarn_packet_t*);

#endif
