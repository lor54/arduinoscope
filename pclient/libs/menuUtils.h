#ifndef MENUUTILS_H
#define MENUUTILS_H

#define MAX_CHN 8

void configureChannels(int *selectedChannels, bool *channels);

void configureSamplingFrequency(unsigned int *samplingFrequency);

void configureTime(unsigned int *time);

void configureOperatingMode(unsigned int *operatingMode);

void setDebugMode(bool *debugMode);

#endif // MENUUTILS_H