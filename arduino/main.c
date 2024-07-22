#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "avr_common/uart.h"
#include "utils.h"

#define LED _BV(PB7)     // = (1 << PB0)

unsigned int ADC_read(unsigned char channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return (ADC);
}

int main(void){
    uart_init();

    DDRB = LED;
    PORTB ^= LED;

    ADMUX=(1<<REFS0);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128

    char rx_buffer[2] = {0, 0};
    bool data_received = false;

    uart_setReceivedBuffer(rx_buffer, 2, &data_received);

    while(1) {
        if(data_received && rx_buffer[0] == CNT_REQUEST_PACKET) {
            while(1) {
                Response resp = {CNT_RESPONSE_PACKET, ADC_read(0), 2, 0x0A};
                uart_SendBytes(&resp, sizeof(resp));

                PORTB ^= LED;

                _delay_ms(1000);
            }

            /*i = ADC_read(1);
            sprintf(string, "Lettura canale 1: %d\n", i);
            while(uart_send_ready());
            _delay_ms(1000);

            i = ADC_read(7);
            sprintf(string, "Lettura canale 7: %d\n", i);
            while(uart_send_ready());
            _delay_ms(1000);*/

            data_received = false;
        } else {
            while(uart_send_ready());
        }
    }
}
