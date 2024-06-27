#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>

int initSerial(const char* devicePath) {
    int serialfd = open(devicePath, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialfd == -1)
        perror("Error opening the serial port");
    else
        fcntl(serialfd, F_SETFL, 0);

    struct termios attribs;
    attribs.c_cflag &= ~O_NONBLOCK;

    attribs.c_iflag &= ~(IXON | IXOFF | IXANY); // Disabilita il controllo del flusso software

    /* get the current settings */
    tcgetattr(serialfd, &attribs);

    /* set the baudrate */
    cfsetospeed(&attribs, B19200); /* outut baudrate */
    cfsetispeed(&attribs, B19200); /* input baudrate */

    /* eventually apply everything for your serialfd descriptor */
    tcsetattr(serialfd, TCSANOW, &attribs);

    fprintf(stdout, "Device is open, attempting read \n");

    fcntl(serialfd, F_SETFL, 0);

    return serialfd;
}