#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

#include "menuUtils.h"

void configureChannels(int *selectedChannels, bool *channels) {
    int numChannel;
    int res;
    do {
        printf("Selected channels: %d\n", *selectedChannels);
        printf("Enter the number of channel to sample (0-7), type exit to exit: ");
        res = scanf("%d", &numChannel);
        if(res > 0) {
            if(numChannel < 0 || numChannel > (MAX_CHN - 1)) {
                printf("Invalid number of channel.\n");
            } else {
                if(!channels[numChannel]) {
                    channels[numChannel] = true;
                    (*selectedChannels)++;
                } else {
                    printf("Channel already selected.\n");
                }
            }
        }
    } while(res > 0 && *selectedChannels < MAX_CHN);
}

void configureSamplingFrequency(unsigned int *samplingFrequency) {
    do {
        printf("Enter the sampling frequency between 1 to 10 (Hz): ");
        scanf("%u", samplingFrequency);
    } while(*samplingFrequency <= 0 || *samplingFrequency > 10);
}

void configureTime(unsigned int *time) {
    do {
        printf("Enter the sampling time (Seconds): ");
        scanf("%u", time);
    } while(*time <= 0);
}

void configureOperatingMode(unsigned int *operatingMode) {
    do {
        printf("Enter the operating mode (0 - Continuous, 1 - Buffered Mode): ");
        scanf("%u", operatingMode);
    } while(*operatingMode != 0 && *operatingMode != 1);
}

void setDebugMode(bool *debugMode) {
    int choice;
    do {
        printf("Enter the operating mode (0 - Continuous, 1 - Buffered Mode): ");
        scanf("%d", &choice);

        if(choice == 0) {
            *debugMode = false;
        } else if(choice == 1) {
            *debugMode = true;
        }
    } while(choice != 0 && choice != 1);
}