#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"

int main() {

    int serialfd = initSerial();

    while(1) {
        char buf[30] = {0};
        read(serialfd, buf, 29);
        fprintf(stdout, "Buffer: %s\n", buf);
    }

    close(serialfd);
    return 0;
}