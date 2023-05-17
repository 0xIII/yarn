#ifndef _YARN_PARSER_H
#define _YARN_PARSER_H

#include "protocol.h"

// "entrypoint" parsing function which takes a pointer to a char-buffer/string in memory
// the issue with passing in a memory reference is that of lifetimes and garbage collection
// how do we ensure that 1) all used memory is freed once it isn't required anymore and 2)
// when handling pointers to strings in memory these pointers don't go UAF once the memory
// has been freed (kinda rusty)
// I have decided on using the pointer-focused approach, so I don't have to keep track of memory
// and the packet should live in memory as a ro for as long as possible anyways
yarn_header_t* parse_packet(unsigned char*);

#endif
