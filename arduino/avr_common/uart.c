#include "uart.h"
// ********************************************************************************
// Includes
// ********************************************************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

// ********************************************************************************
// uart Related
// ********************************************************************************
void uart_init() {
    cli();
    // Set baud rate
    UBRR0H = (uint8_t) (MYUBRR>>8);
    UBRR0L = (uint8_t) MYUBRR;

    uart_rx_buffer = NULL;
    uart_rx_buffer_last = NULL;
    uart_tx_buffer = NULL;
    uart_tx_buffer_last = NULL;
    data_received = NULL;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */  
    sei();
}

void uart_putchar(char* data) {
    uart_tx_buffer = data;
    uart_tx_buffer_last = uart_tx_buffer + 1;

    UCSR0B |= _BV(UDRIE0);
}

char uart_setReceivedBuffer(char* buffer, int size, bool* received) {
	uart_rx_buffer = buffer;
	uart_rx_buffer_last = buffer + size;

    data_received = received;
    UCSR0B |= _BV(RXCIE0);
}

void uart_SendBytes(char *s, int size) {
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

ISR(USART0_RX_vect) {
    char received = UDR0;
    if (uart_rx_buffer < uart_rx_buffer_last){
        *uart_rx_buffer = received;
        uart_rx_buffer++;
    } else {
        *data_received = true;
    }
}

char uart_send_ready() {
	return (uart_tx_buffer < uart_tx_buffer_last);
}