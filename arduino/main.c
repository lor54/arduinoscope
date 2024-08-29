/**
 * @file 
 * @author Lorenzo Thomas Contessa <lorenzocontessa.dev@gmail.com>
 * @date 29/08/2024
 * @copyright 2023 Copyright (c) | Sapienza Space Team, all rights reserved.
 */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "avr_common/uart.h"
#include "utils.h"
#include <avr/sleep.h>

#define LED _BV(PB7)

int done_samples = 0;
uint8_t samples[8];
bool read = false;

unsigned int ADC_read(unsigned char channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    ADCSRA|=(1<<ADSC);
    while(!(ADCSRA & (1<<ADIF)));
    ADCSRA|=(1<<ADIF);
    return (ADC);
}

ISR(TIMER5_COMPA_vect) {
    read = true;
}

int main(void){
    uart_init();

    DDRB = LED;
    PORTB ^= LED;

    TCCR5A = 0;
    TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52);

    ADMUX=(1<<REFS0);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // Prescaler 128

    uint8_t rx_buffer[18];
    bool data_received = false;

    uart_setReceivedBuffer(rx_buffer, 18, &data_received);

    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();

    while(1) {
        if(data_received && rx_buffer[0] == CNT_REQUEST_PACKET) {
            unsigned int samplingFrequency = bytesToUInt(&rx_buffer[9]);
            unsigned int time = bytesToUInt(&rx_buffer[13]);
            unsigned int operatingMode = bytesToUInt(&rx_buffer[17]);

            OCR5A = 15624/samplingFrequency;
            int total_samples = samplingFrequency * time;

            for(int i = 0; i < 8; i++) {
                samples[i] = rx_buffer[i + 1];
            }

            sei();
            TIMSK5 |= _BV(OCIE5A);

            while (done_samples < total_samples) {
                if(read) {
                    for(int i = 0; i < 8; i++) {
                        if(samples[i] != 0xFF) {
                            samples[i] = ADC_read(i);

                            Response resp = {CNT_RESPONSE_PACKET, done_samples, i, 0x0A};
                            uart_SendBytes(&resp, sizeof(resp));
                            while(uart_send_ready());

                            PORTB ^= LED;
                        }
                        _delay_ms(10);
                    }
                    done_samples++;
                    read = false;
                }
                _delay_ms(10);
            }
            
            TIMSK5 &= ~_BV(OCIE5A);

            Response resp = {CNT_END_PACKET, done_samples, 0x0A, 0x0A};
            uart_SendBytes(&resp, sizeof(resp));
            while(uart_send_ready());

            data_received = false;
        }
        sleep_mode();
    }
}
