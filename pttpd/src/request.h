#ifndef _YARNREQUEST_H
#define _YARNREQUEST_H

enum yarn_methods {
    GET,
    PUT,
    UPDATE
};

struct yarn_request {
    char*   method;
    char*   address;
    int     version;
};

int parse(char* string) {
    
}

#endif
