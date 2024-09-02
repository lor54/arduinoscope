# Arduinoscope
This software is designed to sample analog signals from AT Mega 2560 channels through serial communication. It offers two operational modes: continuous sampling and buffered sampling. Users can configure channels, sampling frequency, and acquisition time via an interactive menu.

**Sampling Modes**

1. Continuous Sampling
In this mode, data is collected and saved in a text file (adcContinuous.txt) in real-time until the end-of-sampling packet is received. The function handles data flow and updates the file line by line.
2. Buffered Sampling
In this mode, data is accumulated in a buffer and written to a file (adcBuffered.txt) only when the buffer is full. A trigger on a digital pin (e.g., GND on Pin 12) is awaited to start sampling.

## Software execution

**Building and running commands**

Make sure to have correctly set the serial port inside the main function as initSerial parameter.

Building the project for the arduino side, command to be executed inside the arduino folder:
```sh
make main.hex
```
Make sure to have correctly set the AVRDUDE_PORT inside the arduino/avr_common/avr.mk file.

Building and run the project for the pc side when inside this pclient folder:
```sh
make
./pclient
```

Plot sample files using gnuplot:
```sh
./plot.sh filename.txt
```
**Output files:**
- adcContinuous.txt: Contains data collected in continuous mode.
- adcBuffered.txt: Contains data collected in buffered mode.