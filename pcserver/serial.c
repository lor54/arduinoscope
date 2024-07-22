#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int initSerial(const char* devicePath) {    
    int serialfd = open(devicePath, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialfd == -1) {
        perror("Error opening the serial port");
        return -1;
    }

    fcntl(serialfd, F_SETFL, 0);
    struct termios attribs;
    if (tcgetattr(serialfd, &attribs) == -1) {
        perror("Errore durante tcgetattr");
        close(serialfd);
        return -1;
    }

    cfmakeraw(&attribs);

    cfsetospeed(&attribs, B19200); /* outut baudrate */
    cfsetispeed(&attribs, B19200); /* input baudrate */

    attribs.c_cflag &= ~PARENB; // No parity
    attribs.c_cflag &= ~CSTOPB; // 1 stop bit
    attribs.c_cflag &= ~CSIZE;
    attribs.c_cflag |= CS8;     // 8 data bits
    attribs.c_cflag &= ~CRTSCTS; // No flow control
    attribs.c_cflag |= CREAD | CLOCAL; // Enable receiver, Ignore modem control lines

    attribs.c_lflag &= ~ISIG;
    attribs.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // No software flow control
    attribs.c_oflag &= ~OPOST; // Raw output

    // Set read timeout
    attribs.c_cc[VMIN] = 0;
    attribs.c_cc[VTIME] = 10; // Timeout in deciseconds

    tcflush(serialfd, TCIFLUSH);

    if (tcsetattr(serialfd, TCSANOW, &attribs) == -1) {
        perror("Errore durante tcsetattr");
        close(serialfd);
        return -1;
    }

    printf("Device is open, attempting read \n");
    return serialfd;
}