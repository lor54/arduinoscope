#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "lib/uart.h"

int main(void){
    printf_init();

    while(1) {
        printf("Hello World\n");
        _delay_ms(1000);
    }
}
