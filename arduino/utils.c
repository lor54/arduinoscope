#include "utils.h"

void cleanBuffer(char* rx_buffer, int size) {
    for(int i = 0; i < size; i++) {
        rx_buffer[i] = 0;
    }
}