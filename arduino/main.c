#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "avr_common/uart.h"

unsigned int ADC_read(unsigned char channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return (ADC);
}

int main(void){
    usart_init();

    char buffer[6];
    while(1) {
        snprintf(buffer, 6, "Ciao\n");

        usart_pstr(buffer, sizeof(buffer));
        while(uart_send_ready());
        
        _delay_ms(1000);
    }

    ADMUX=(1<<REFS0);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128

    while(1) {        
        char prova[5];
        usart_getchar(&prova, sizeof(prova));
        printf("%c\n", prova);
        
        int i = ADC_read(0);
        printf("Lettura canale 0: %d\n", i);
        _delay_ms(1000);

        i = ADC_read(1);
        printf("Lettura canale 1: %d\n", i);
        _delay_ms(1000);

        i = ADC_read(7);
        printf("Lettura canale 7: %d\n", i);
        _delay_ms(1000);
    }
}
