#pragma once
#include <stdbool.h>

volatile char* uart_rx_buffer;
volatile char* uart_rx_buffer_last;
volatile char* uart_tx_buffer;
volatile char* uart_tx_buffer_last;
volatile bool* data_received;

// ********************************************************************************
// Macros and Defines
// ********************************************************************************
#define BAUD 19200
#define MYUBRR (F_CPU / (16UL * BAUD)) - 1


// ********************************************************************************
// Function Prototypes
// ********************************************************************************
void uart_init();
void uart_putchar(char* data);
char uart_setReceivedBuffer(char* buffer, int size, bool* usart_getchar);
void uart_pstr (char *s, int size);
char uart_send_ready(void);