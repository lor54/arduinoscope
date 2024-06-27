#include "uart.h"
// ********************************************************************************
// Includes
// ********************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

// ********************************************************************************
// usart Related
// ********************************************************************************
void usart_init() {
    cli();
    // Set baud rate
    UBRR0H = (uint8_t) (MYUBRR>>8);
    UBRR0L = (uint8_t) MYUBRR;

    uart_rx_buffer = NULL;
    uart_rx_buffer_last = NULL;
    uart_tx_buffer = NULL;
    uart_tx_buffer_last = NULL;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX */  
    sei();
}

void usart_putchar(char* data) {
    uart_tx_buffer = data;
    uart_tx_buffer_last = uart_tx_buffer + 1;

    UCSR0B |= _BV(UDRIE0);
}

void usart_pstr(char *s, int size) {
    uart_tx_buffer = s;
    uart_tx_buffer_last = uart_tx_buffer + size;

    UCSR0B |= _BV(UDRIE0);
}

ISR(USART0_UDRE_vect) {
    char transmit;
    if(uart_tx_buffer < uart_tx_buffer_last) {
        transmit = *uart_tx_buffer;
        UDR0 = transmit;
        uart_tx_buffer++;
    } else {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

char uart_send_ready() {
	return (uart_tx_buffer < uart_tx_buffer_last);
}