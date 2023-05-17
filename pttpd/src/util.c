#ifndef _YARN_UTIL
#define _YARN_UTIL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// parse a raw string into an array of tokens
// (the maximum number of parsed tokens is determined by token_max)
int parse_to_tokenstream(int token_max, char** tokens, char* raw, char* delimiter) {
    char* token;
    int tcnt = 0;
    token = strtok(raw, delimiter);
    while(token != NULL && tcnt < token_max) {
        tokens[tcnt] = token;
        token = strtok(NULL, delimiter);
        tcnt++;
    }
    
    return tcnt;
}

int herror(FILE* handle, int ret, char* error) {
    if (ret < 0) {
        fprintf(handle, "[ERROR] %s\n", error);
        exit(EXIT_FAILURE);
    }
    return ret;
}

#endif
