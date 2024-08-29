#include "utils.h"

void uintToBytes(unsigned int value, uint8_t* data) {
    memcpy(data, &value, sizeof(unsigned int));
}

int getFirstChannelIndex(bool* sampleChannels, int numChannels) {
    for(int i = 0; i < numChannels; i++) {
        if(sampleChannels[i]) {
            return i;
        }
    }
    
    return -1;
}