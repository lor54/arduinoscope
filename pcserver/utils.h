enum Packets {
    CNT_REQUEST_PACKET = 0x81,
    BUF_REQUEST_PACKET = 0x82,
    CNT_RESPONSE_PACKET = 0x83,
    BUF_RESPONSE_PACKET = 0x84,
    ERROR_PACKET = 0x85
};

struct response {
    enum Packets packet_type;
    unsigned int value;
    unsigned char channel;
    unsigned char lastByte;
};
typedef struct response Response;