#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include "avr_common/uart.h"
#include "utils.h"
#include <avr/sleep.h>
#include "main.h"

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

            OCR5A = 15625/samplingFrequency;
            int total_samples = samplingFrequency * time;

            for(int i = 0; i < 8; i++) {
                samples[i] = rx_buffer[i + 1];
            }

            sei();
            continuousSampling(total_samples);

            data_received = false;
        } else if(data_received && rx_buffer[0] == BUF_REQUEST_PACKET) {
            unsigned int samplingFrequency = bytesToUInt(&rx_buffer[9]);
            unsigned int time = bytesToUInt(&rx_buffer[13]);

            OCR5A = 15624/samplingFrequency;
            int total_samples = samplingFrequency * time;

            for(int i = 0; i < 8; i++) {
                samples[i] = rx_buffer[i + 1];
            }



            sei();
            bufferedSampling(total_samples);

            data_received = false;
        }
        sleep_mode();
    }
}

void continuousSampling(int total_samples) {
    TIMSK5 |= _BV(OCIE5A);

    while (done_samples < total_samples) {
        if(read) {
            for(int i = 0; i < 8; i++) {
                if(samples[i] != 0xFF) {
                    samples[i] = ADC_read(i);

                    Response resp = {CNT_RESPONSE_PACKET, samples[i], i};
                    uart_SendBytes(&resp, sizeof(resp));
                    while(uart_send_ready());

                    _delay_ms(10);
                }
            }
            done_samples++;
            read = false;
        }
        _delay_ms(10);
    }
    
    TIMSK5 &= ~_BV(OCIE5A);

    Response resp = {CNT_END_PACKET, done_samples, 0x0A};
    uart_SendBytes(&resp, sizeof(resp));
    while(uart_send_ready());
}

void bufferedSampling(int total_samples) {
    int bufSamples[8][5];

    const uint8_t mask = (1 << 6);
    DDRB &= ~mask;
    PORTB |= mask;

    while((PINB&mask)!=0) {
        _delay_ms(10);
    }
    Response resp2 = {BUF_RESPONSE_BEGIN, 0x0A, 0x0A};
    uart_SendBytes(&resp2, sizeof(resp2));
    while(uart_send_ready());

    TIMSK5 |= _BV(OCIE5A);

    int j = 0;
    while (done_samples < total_samples) {
        if(read) {
            if(j >= 4) {
                for(int m = 0; m < 8; m++) {
                    if(samples[m] != 0xFF) {
                        uint8_t buf[28];
                        buf[0] = BUF_RESPONSE_PACKET;
                        int l = 1;
                        intToBytes(m, &buf[l]);
                        l+=4;
                        for(int k = 0; k < 5; k++) {
                            intToBytes(bufSamples[m][k], &buf[l]);
                            l += 4;
                        }
                        buf[l] = 0x0A;

                        uart_SendBytes(&buf, 28);
                        while(uart_send_ready());

                        _delay_ms(10);
                    }
                }

                j = 0;
            }

            for(int i = 0; i < 8; i++) {
                if(samples[i] != 0xFF) {
                    bufSamples[i][j] = ADC_read(i);
                } else {
                    bufSamples[i][j] = 0xFF;
                }
            }
            j++;
            done_samples++;
            read = false;
        }
        _delay_ms(1);
    }

    TIMSK5 &= ~_BV(OCIE5A);

    Response resp = {BUF_END_PACKET, done_samples, 0x0A};
    uart_SendBytes(&resp, sizeof(resp));
    while(uart_send_ready());
}