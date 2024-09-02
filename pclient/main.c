#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libs/serial.h"
#include "libs/utils.h"
#include "libs/file.h"
#include "libs/menuUtils.h"
#include "main.h"

bool debugMode = true;

void sampleChannels(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time, unsigned int operatingMode) {
    printf("Sampling %d channels at %d Hz...\n", numChannels, samplingFrequency);

    uint8_t data[18];
    data[0] = operatingMode == 0 ? CNT_REQUEST_PACKET : BUF_REQUEST_PACKET;
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

    if (operatingMode == 0) continuousSampling(serialfd, sampleChannels, numChannels, samplingFrequency, time);
    else bufferedSampling(serialfd, sampleChannels, numChannels, samplingFrequency, time);
}

void continuousSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time) {
    openFile("adcContinuous.txt", MAX_CHN, sampleChannels);
    int firstChannelIndex = getFirstChannelIndex(sampleChannels);

    bool cexit = false;
    while(!cexit) {
        unsigned char buffer[10];
        int datasize = 0;
        do {
            datasize = read(serialfd, &buffer, sizeof(buffer));

            if(buffer[0] == CNT_END_PACKET) {
                cexit = true;
            }
        } while (buffer[0] != CNT_RESPONSE_PACKET && !cexit);

        if(datasize > 0 && buffer[0] != CNT_END_PACKET) {
            if(debugMode) printf("Buffer: %x, %u, %u\n", buffer[0], buffer[1], buffer[3]);

            if(buffer[3] == firstChannelIndex) {
                writeNewLine();
            }
            writeToFile(buffer[1]);
        }

        if(cexit) {
            printf("Sampling completed.\n");
            exit(0);
        }
    }

    closeFile();
}

void bufferedSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time) {
    openFile("adcBuffered.txt", numChannels, sampleChannels);

    int channelData[MAX_CHN][5];
    for(int i = 0; i < MAX_CHN; i++) {
        if(sampleChannels[i]) {
            channelData[i][0] = 0;
        } else {
            channelData[i][0] = -1;
        }
    }

    bool triggerReceived = false;
    printf("Waiting for GND trigger on Digital Pin 12...\n");
    int count = 0;
    bool cexit = false;
    while(!cexit) {
        if(count >= numChannels) {
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < MAX_CHN; j++) {
                    if(sampleChannels[j]) writeToFile(channelData[j][i]);
                }
                writeNewLine();
            }
            count = 0;
        }

        unsigned char buffer[28];
        int datasize = 0;
        do {
            datasize = read(serialfd, &buffer, sizeof(buffer));

            if(buffer[0] == BUF_END_PACKET) {
                cexit = true;
            } else if(!triggerReceived && buffer[0] == BUF_RESPONSE_BEGIN) {
                printf("Trigger received!\n");
                triggerReceived = true;
            }
        } while (buffer[0] != BUF_RESPONSE_PACKET && !cexit);

        if(datasize > 0 && buffer[0] != BUF_END_PACKET) {
            if(debugMode) printf("Buffer: %x, %d, %d, %d, %d, %d, %d\n", buffer[0], buffer[1], buffer[5], buffer[9], buffer[13], buffer[17], buffer[21]);

            int l = 0;
            for(int i = 0; i < 5; i++) {
                channelData[buffer[1]][i] = buffer[5 + l];
                l += 4;
            }
            count++;
        }

        if(cexit) {
            printf("Sampling completed.\n");
            closeFile();
            exit(0);
        }
    }
}

void openMenu(int serialfd) {
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
        printf("}, Sampling Frequency: %d Hz, Time: %d seconds, Operating Mode: %s, Debug Mode: %s\n", samplingFrequency, time, operatingMode ? "Buffered" : "Continuous", debugMode ? "Enabled" : "Disabled");
        printf("\nMenu:\n");
        printf("1. Start Sampling\n");
        printf("2. Configure Channels\n");
        printf("3. Configure Sampling Frequency\n");
        printf("4. Configure Time\n");
        printf("5. Configure Operating Mode\n");
        printf("6. Configure Debug Mode\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        while (scanf("%d", &choice) != 1 || choice < 1 || choice > 7) {
            printf("Invalid input. Please enter a number between 1 and 7: ");
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
                setDebugMode(&debugMode);
                system("clear");
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
        while(getchar() != '\n');
    } while (choice != 6);
}

int main() {
    int serialfd = initSerial("/dev/tty.usbserial-110");
    if (serialfd == -1) {
        perror("Error during serial port initialization, exiting...");
        return -1;
    }

    openMenu(serialfd);
    return 0;
}