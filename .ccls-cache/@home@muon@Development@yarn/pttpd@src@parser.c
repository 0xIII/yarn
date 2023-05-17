#include <stdlib.h>
#include <malloc.h>

#include "parser.h"
#include "util.c"

yarn_header_t* parse_packet(unsigned char* raw) {
    yarn_header_t* header = malloc(sizeof(yarn_header_t)); 
}

int parse_packet_hdrline(yarn_header_t* hdr, slice_t hdrline) {
     
}
