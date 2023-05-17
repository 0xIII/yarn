#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "util.c"

// parse a packet into the packet structure
yarn_packet_t* to_packet(char* raw) {
    char* str_cpy = raw;
    yarn_packet_t* packet = malloc(sizeof(yarn_packet_t)); 
    
    split_t* req_line = split_string(str_cpy, "\r\n");
    split_t* method_target = split_char(req_line->first, ' ');
    packet->method = method_target->first;
    split_t* target_version = split_char(method_target->second, ' ');
    packet->target = target_version->first;
    packet->version = target_version->second;

    return packet;
}

// create a new map
yarn_headermap_t* map() {
   yarn_headermap_t* map = malloc(sizeof(yarn_headermap_t));
   memset(map, 0, sizeof(yarn_headermap_t));
   return map;
}

// add an entry to a map
// TODO: check if header is already present -> discard
yarn_headermap_t* add(yarn_headermap_t* map, char* key, char* value) {
    if (map->length < MAX_LENGTH) {
        // create a new yarn_header_t
        yarn_header_t* header = malloc(sizeof(yarn_header_t));
        header->key = key;
        header->value = value;

        // if all requirements are met it will be added to the headermap
        if (map->head == NULL || map->tail == NULL) {
            map->head = header;
            map->head->next = map->tail;
            map->tail = header;
        } else {
            map->tail->next = header;
            map->tail = header;
        }
        
        map->length++;
    }
    return map;
}

yarn_headermap_t* remove_key(yarn_headermap_t* map, char* key) {
    yarn_header_t* current_block = map->head;
    yarn_header_t* previous_block = NULL;
    while (current_block != NULL) {
        if(strcmp(current_block->key, key) == 0) {
            if(previous_block == NULL) {
                map->head = current_block->next;
            } else {
                previous_block->next = current_block->next;
            }
            free(current_block);
            break;
        }
        previous_block = current_block;
        current_block = current_block->next;
    }

    return map;
}
    
void free_map(yarn_headermap_t* map) {
    yarn_header_t* current_block = map->head;
    while (current_block != NULL) {
        printf("current_block: %p\n", current_block);
        current_block = current_block->next;
        free(current_block);
    }
    free(map);
}

void map_print(yarn_headermap_t* map) {
    yarn_header_t* current_block = map->head;
    while (current_block != NULL) {
        printf("%s\t|\t%s\n", current_block->key, current_block->value);
        current_block = current_block->next;
    }
}
