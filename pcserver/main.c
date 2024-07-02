#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHN 8

void sampleChannels(int serialfd, bool* sampleChannels, int numChannels, int samplingFrequency) {
    printf("Sampling %d channels at %d Hz...\n", numChannels, samplingFrequency);

    char data[] = "0x81";

    while(1) {
        if (write(serialfd, data, sizeof(data)) == -1) {
            perror("Errore durante la scrittura sulla porta seriale");
        }

        printf("CIao\n");
        sleep(1);

        char buf[30] = {0};
        read(serialfd, buf, 29);
        fprintf(stdout, "Buffer: %s\n", buf);
        sleep(1);
    }

    sleep(1);
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

void configureSamplingFrequency(int *samplingFrequency) {
    printf("Enter the sampling frequency (Hz): ");
    scanf("%d", samplingFrequency);
}


int main() {

    int serialfd = initSerial("/dev/tty.usbserial-110");

    int choice;
    int samplingFrequency = 1000;
    int selectedChannels = 0;
    bool channels[MAX_CHN] = {false};

    system("clear");
    do {
        printf("\nMenu:\n");
        printf("1. Start Sampling\n");
        printf("2. Configure Channels\n");
        printf("3. Configure Sampling Frequency\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            printf("Invalid input. Please enter a number between 1 and 4: ");
            while(getchar() != '\n');
        }

        switch (choice) {
            case 1:
                if (selectedChannels > 0) {
                    sampleChannels(serialfd, channels, selectedChannels, samplingFrequency);
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
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
        while(getchar() != '\n');
    } while (choice != 4);

    return 0;
}