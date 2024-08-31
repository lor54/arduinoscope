#include "file.h"
#include <stdio.h>

static FILE *file = NULL;

void openFile(const char *filename, int numChannels, bool* sampleChannels) {
    file = fopen(filename, "w");
    if (file == NULL) {
        return;
    }
    
    for (int i = 0; i < numChannels; i++) {
        if(sampleChannels[i]) fprintf(file, "Channel %d\t", i);
    }
    fprintf(file, "\n");
}

void closeFile() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

void writeToFile(int adcValue) {
    if (file == NULL) {
        return;
    }

    fprintf(file, "%d\t", adcValue);
}

void writeNewLine() {
    if (file == NULL) {
        return;
    }

    fprintf(file, "\n");
}