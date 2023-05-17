#ifndef _YARN_UTIL
#define _YARN_UTIL

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>

typedef struct split {
    char* first;
    char* second;
} split_t;

// Take a string and split off everything before a specified character
split_t* split_char(char* string, char separator) {
    size_t cnt = 0;
    while (string[cnt] != separator && string[cnt] != '\0')
        cnt++;

    char* first = NULL;
    first = malloc(sizeof(char)*(cnt+1));
    strncpy(first, string, cnt);
    first[cnt+1] = '\0';
    
    split_t* split = malloc(sizeof(split_t));
    split->first = first;
    int offset = cnt+1;
    if (offset < strlen(string)) {
        split->second = string+offset;
    } else {
        split->second = NULL;
    }

    return split;
}

// Take a string and split off everything before a specified string
split_t* split_string(char* string, char* separator) {
    size_t cnt = 0;
    while (strncmp(string+cnt*sizeof(char), separator, strlen(separator)) > 0 && string[cnt] != '\0')
        cnt++;
    char* first = NULL;
    first = malloc(sizeof(char)*(cnt+1));
    strncpy(first, string, cnt);
    first[cnt+1] = '\0';
    
    split_t* split = malloc(sizeof(split_t));
    split->first = first;
    int offset = cnt+strlen(separator);
    if (offset < strlen(string)) {
        split->second = string+offset;
    } else {
        split->second = NULL;
    }

    return split;
}

int herror(FILE* handle, int ret, char* error) {
    if (ret < 0) {
        fprintf(handle, "[ERROR] %s\n", error);
        exit(EXIT_FAILURE);
    }
    return ret;
}

uint8_t to_int(char c) {
    if(64 < (uint8_t)c) { return (uint8_t)c-55; }
    else { return (uint8_t)c-48; }

    return -1;
}

int compress_string(char* string, char* compressed) {
    char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* test = "669C164F44198B43B7175AC0DFF496FE43393717428747CCB44C294FBEACA6E0";
    
    int carry = to_int(test[0]);
    for(int i = 1; i < 64; i++) {
        int a = carry >> (4-(3%4));
        int b = to_int(test[i]) << 1;
        int out = a | b;
    }

    return 0;
}

int decompress_string(char* compressed, char* string) {
    return 0;
}

#endif
