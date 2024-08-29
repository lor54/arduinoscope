#include <stdint.h>
#include <string.h>

enum Packets {
    CNT_REQUEST_PACKET = 0x81,
    BUF_REQUEST_PACKET = 0x82,
    CNT_RESPONSE_PACKET_BEGIN = 0x83,
    CNT_RESPONSE_PACKET = 0x84,
    BUF_RESPONSE_PACKET = 0x85,
    CNT_END_PACKET = 0x86,
    ERROR_PACKET = 0x87
};

void uintToBytes(unsigned int value, uint8_t* data);