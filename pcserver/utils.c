#include "utils.h"

void uintToBytes(unsigned int value, uint8_t* data) {
    memcpy(data, &value, sizeof(unsigned int));
}