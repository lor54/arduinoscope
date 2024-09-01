#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

void sampleChannels(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time, unsigned int operatingMode);

void continuousSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time);

void bufferedSampling(int serialfd, bool* sampleChannels, int numChannels, unsigned int samplingFrequency, unsigned int time);

void configureChannels(int *selectedChannels, bool *channels);

void configureSamplingFrequency(unsigned int *samplingFrequency);

void configureTime(unsigned int *time);

void configureOperatingMode(unsigned int *operatingMode);

void openMenu(int serialfd);

#endif // MAIN_H