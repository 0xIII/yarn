#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../protocol.c"

int main(int argc, char** argv) {
    char example[200] = "GET 669C164F44198B4B7175AC0DFF496FE43393717428747CCB44C294FBEACA6E0 YARN1.0\r\nHEADERA: HEADERINFO\r\nHEADERB: HEADERINFO\r\n\r\nBODY\r\n";
    
    yarn_packet_t* packet = to_packet(example);
    assert(packet != NULL);
    
    map_print(packet->headers);

    return 0;
}
