#include <stdint.h>
#include <stdbool.h>

void printBufferHex(const char* buffer, size_t length);

void sampleChannels(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time, uint8_t operatingMode);

void continuosSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time);

void bufferedSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time);

void configureChannels(int *selectedChannels, bool *channels);

void configureSamplingFrequency(unsigned int *samplingFrequency);

void configureTime(unsigned int *time);

void configureOperatingMode(unsigned int *operatingMode);
