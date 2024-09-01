#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

void openFile(const char *filename, int numChannels, bool* sampleChannels);

void closeFile();

void writeToFile(int adcValue);

void writeNewLine();

#endif // FILE_H