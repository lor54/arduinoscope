#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"
#include "utils.h"
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHN 8

void printBufferHex(const char* buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x ", (unsigned char)buffer[i]);
    }
    printf("\n");
}

void sampleChannels(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time, uint8_t operatingMode) {
    printf("Sampling %d channels at %d Hz...\n", numChannels, samplingFrequency);

    uint8_t data[18];
    data[0] = CNT_REQUEST_PACKET;
    int j = 1;
    for(int i = 0; i < MAX_CHN; i++) {
        if(sampleChannels[i]) {
            data[j] = i;
        } else {
            data[j] = 0xFF;
        }
        j++;
    }
    uintToBytes(samplingFrequency, &data[j]);
    j+=4;
    uintToBytes(time, &data[j]);
    j+=4;
    data[j] = operatingMode;

    if (write(serialfd, data, 18) == -1) {
        perror("Errore durante la scrittura sulla porta seriale");
    }
    usleep(100);
    if (write(serialfd, data, 18) == -1) {
        perror("Errore durante la scrittura sulla porta seriale");
    }

    bool exit = false;
    while(!exit) {
        unsigned char buffer[10];
        int datasize = 0;
        do {
            datasize = read(serialfd, &buffer, sizeof(buffer));

            if(buffer[0] == CNT_END_PACKET) {
                exit = true;
            }
        } while (buffer[0] != CNT_RESPONSE_PACKET && !exit);

        if(datasize > 0) printf("Buffer: %x, %u, %u\n", buffer[0], buffer[1], buffer[3]);
    }
}

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
        printf("Enter the sampling frequency (Hz): ");
        scanf("%u", samplingFrequency);
    } while(*samplingFrequency <= 0);
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


int main() {

    int serialfd = initSerial("/dev/tty.usbserial-110");

    int choice;
    unsigned int samplingFrequency = 1, time = 15;
    unsigned int operatingMode = 0;
    int selectedChannels = 0;
    bool channels[MAX_CHN] = {false};

    system("clear");
    do {
        int count = 0;
        printf("Actual settings: Selected Channels: {");
        for(int i = 0; i < MAX_CHN; i++) {
            if(channels[i]) {
                if(count < 1) {
                    printf("%d", i);
                    count++;
                }
                else printf(", %d", i);
            }
        }
        printf("}, Sampling Frequency: %d Hz, Time: %d seconds, Operating Mode: %s\n", samplingFrequency, time, operatingMode ? "Buffered" : "Continuous");
        printf("\nMenu:\n");
        printf("1. Start Sampling\n");
        printf("2. Configure Channels\n");
        printf("3. Configure Sampling Frequency\n");
        printf("4. Configure Time\n");
        printf("5. Configure Operating Mode\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 6) {
            printf("Invalid input. Please enter a number between 1 and 4: ");
            while(getchar() != '\n');
        }

        switch (choice) {
            case 1:
                if (selectedChannels > 0) {
                    sampleChannels(serialfd, channels, selectedChannels, samplingFrequency, time, operatingMode);
                } else {
                    printf("Please configure channels first.\n");
                }                
                break;
            case 2:
                configureChannels(&selectedChannels, channels);
                system("clear");
                break;
            case 3:
                configureSamplingFrequency(&samplingFrequency);
                system("clear");
                break;
            case 4:
                configureTime(&time);
                system("clear");
                break;
            case 5:
                configureOperatingMode(&operatingMode);
                system("clear");
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
        while(getchar() != '\n');
    } while (choice != 6);

    return 0;
}