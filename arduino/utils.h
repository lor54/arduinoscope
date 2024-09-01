#include "stdint.h"
#include <string.h>

void cleanBuffer(char* rx_buffer, int size);

unsigned int bytesToUInt(uint8_t* data);

void intToBytes(int value, uint8_t* data);

enum Packets {
    CNT_REQUEST_PACKET = 0x81,
    BUF_REQUEST_PACKET = 0x82,
    CNT_RESPONSE_PACKET_BEGIN = 0x83,
    CNT_RESPONSE_PACKET = 0x84,
    BUF_RESPONSE_PACKET = 0x85,
    CNT_END_PACKET = 0x86,
    BUF_END_PACKET = 0x87,
    BUF_RESPONSE_BEGIN = 0x88
};

struct response {
    enum Packets packet_type;
    unsigned int value;
    unsigned char channel;
};
typedef struct response Response;