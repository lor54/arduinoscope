#include <avr/io.h>
#include "utils.h"

void cleanBuffer(char* rx_buffer, int size) {
    for(int i = 0; i < size; i++) {
        rx_buffer[i] = 0;
    }
}

unsigned int bytesToUInt(uint8_t* data) {
    unsigned int value;
    memcpy(&value, data, sizeof(unsigned int));
    return value;
}

void intToBytes(int value, uint8_t* data) {
    memcpy(data, &value, sizeof(int));
}

void enableSquareWave() {
  DDRH |= _BV(PH4);
  PORTH &= ~_BV(PH4);
  
  TCCR4A = 0;
  TCCR4B = (1 << WGM52) | (1 << CS50) | (1 << CS52); // prescaler = 1024
  
  OCR4A = 15625/1; // 10 Hz
  
  TIMSK4 |= _BV(OCIE4A);
}