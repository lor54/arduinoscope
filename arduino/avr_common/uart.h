#pragma once

volatile char* uart_rx_buffer;
volatile char* uart_rx_buffer_last;
volatile char* uart_tx_buffer;
volatile char* uart_tx_buffer_last;

// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19200
#define MYUBRR (F_CPU / (16UL * BAUD)) - 1


// ********************************************************************************
// Function Prototypes
// ********************************************************************************
void usart_init();
void usart_putchar(char* data);
void usart_pstr (char *s, int size);
unsigned char usart_kbhit(void);
char uart_send_ready(void);

void printf_init(void);
